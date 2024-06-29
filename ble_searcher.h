#ifndef BLE_SEARCHER_H
#define BLE_SEARCHER_H

#include <stdio.h>
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <inttypes.h>

#define MAX_DEVICES 20
#define MAX_LENGHT_NAME 8

struct device
{
    bd_addr_t address;
    char name[MAX_LENGHT_NAME];
    uint8_t size_name;
};

static btstack_packet_callback_registration_t hci_event_callback_registration;
static void packet_handler_scanner(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
static void device_add(bd_addr_t address, char *name, uint8_t size_name);
static int8_t search_device(bd_addr_t addr);


void ble_scan_devices(void);
#endif