/**
 * Copyright (c) 2023 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <inttypes.h>

#include "ble_searcher.h"

int main()
{
    stdio_init_all();
    if (cyw43_arch_init())
    {
        printf("failed to initialise cyw43_arch\n");
        return -1;
    }
    hci_power_control(HCI_POWER_ON);

    ble_scan_devices();

    while (1)
    {
        /* code */
    }

    return 0;
}
