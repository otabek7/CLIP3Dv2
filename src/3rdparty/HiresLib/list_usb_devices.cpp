// #include "list_usb_devices.h"

// #include <stdio.h>
// #include "hidapi.h"
// #include <stdlib.h>

// /*
//  *
//  * This module lists all connected USB devices using the HIDAPI library.
//  *
//  * Copyright (C) 2024 Your Name
//  * ALL RIGHTS RESERVED
//  *
//  */


// void list_usb_devices() {
//     struct hid_device_info *devs, *current_device;
//     int res;

//     // Initialize HIDAPI
//     res = hid_init();
//     if (res < 0) {
//         fprintf(stderr, "Failed to initialize HIDAPI.\n");
//         return;
//     }
//     const char* target_path1 = "\\\\?\\hid#dell0816&col05#5&2c0c5c6d&0&0004#{4d1e55b2-f16f-11cf-88cb-001111000030}";

//     // Enumerate HID devices
//     devs = hid_enumerate(0x0, 0x0); // Use 0x0 for VID and PID to list all devices
//     current_device = devs;

//     // Print details of each device
//     while (current_device) {
//         printf("Manufacturer String: %ls\n", current_device->manufacturer_string);
//         printf("Device Path: %s\n", current_device->path);
//         printf("Vendor ID: %04hx\n", current_device->vendor_id);
//         printf("Product ID: %04hx\n", current_device->product_id);
//         // printf("Release Number: %04hx\n", current_device->release_number);
//         // printf("Product String: %ls\n", current_device->product_string);
//         // printf("Serial Number: %ls\n", current_device->serial_number);
//         // printf("Interface Number: %d\n", current_device->interface_number);
//         // printf("Usage Page: %04hx\n", current_device->usage_page);
//         // printf("Usage: %04hx\n", current_device->usage);

//         current_device = current_device->next;
//     }

//     // Free the device list
//     hid_free_enumeration(devs);

//     // Exit HIDAPI
//     hid_exit();
// }

// int main() {
//     list_usb_devices();
//     return 0;
// }