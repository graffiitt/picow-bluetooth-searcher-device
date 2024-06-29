#include "ble_searcher.h"

struct device devices[MAX_DEVICES];
uint8_t deviceCount = 0;

static void dump_advertisement_data(const uint8_t *adv_data, uint8_t adv_size, bd_addr_t address)
{
    ad_context_t context;
    for (ad_iterator_init(&context, adv_size, (uint8_t *)adv_data); ad_iterator_has_more(&context); ad_iterator_next(&context))
    {
        uint8_t data_type = ad_iterator_get_data_type(&context);
        uint8_t size = ad_iterator_get_data_len(&context);
        const uint8_t *data = ad_iterator_get_data(&context);
        int i;

        if ((data_type == BLUETOOTH_DATA_TYPE_SHORTENED_LOCAL_NAME) || (data_type == BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME))
        {
            for (i = 0; i < size; i++)
            {
                printf("%c", (char)(data[i]));
            }
            device_add(address, (char *)data, size);
            break;
        }
    }
    printf("\n");
    printf("\n");
}

static void packet_handler_scanner(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    UNUSED(channel);
    UNUSED(size);

    if (packet_type != HCI_EVENT_PACKET)
        return;

    printf("pacet handler\n");

    bd_addr_t address;
    uint8_t length;
    const uint8_t *data;

    switch (hci_event_packet_get_type(packet))
    {
    case GAP_EVENT_ADVERTISING_REPORT:
        gap_event_advertising_report_get_address(packet, address);
        length = gap_event_advertising_report_get_data_length(packet);
        data = gap_event_advertising_report_get_data(packet);
        printf("Advertisement (legacy) event: addr %s, , data[%u] ", bd_addr_to_str(address), length);
        printf_hexdump(data, length);
        dump_advertisement_data(data, length, address);

        break;
    default:
        break;
    }
}

void device_add(bd_addr_t address, char *name, uint8_t size_name)
{
    if (search_device(address) == 1)
    {
        printf("device not add\n");
        return;
    }
    if (size_name > MAX_LENGHT_NAME)
    {
        size_name = MAX_LENGHT_NAME - 1;
    }
    for (uint8_t i = 0; i < size_name; i++)
    {
        devices[deviceCount].name[i] = name[i];
    }
    devices[deviceCount].size_name = size_name;
    bd_addr_copy(devices[deviceCount].address, address);
    if (deviceCount < MAX_DEVICES)
        deviceCount++;
}

int8_t search_device(bd_addr_t addr)
{
    for (uint8_t i = 0; i < MAX_DEVICES; i++)
    {
        if (bd_addr_cmp(addr, devices[i].address) == 0)
        {
            printf("    adress equal find\n");
            return 1;
        }
    }
    return 0;
}

static void cleaning_devices_struct()
{
    deviceCount = 0;
    bd_addr_t ad = {0, 0, 0, 0, 0, 0};
    for (uint8_t i = 0; i < MAX_DEVICES; i++)
    {
        bd_addr_copy(devices[i].address, ad);
        devices[i].size_name = 0;
        for (uint8_t j = 0; j < MAX_LENGHT_NAME; j++)
        {
            devices[i].name[j] = '\0';
        }
    }
}

void ble_scan_devices(void)
{
    cleaning_devices_struct();

    gap_set_scan_parameters(1, 48, 48);
    gap_start_scan();

    hci_event_callback_registration.callback = &packet_handler_scanner;
    hci_add_event_handler(&hci_event_callback_registration);

    sleep_ms(10000);
    gap_stop_scan();
    hci_remove_event_handler(&hci_event_callback_registration);

    printf("output   %d\n", deviceCount);
    for (uint8_t i = 0; i < MAX_DEVICES; i++)
    {
        printf("%d:%s\n", i, devices[i].name);
    }
}