//
//  xbox_one_controller_report_descriptor.h
//  xbox_one_controller
//
//  Created by Loic Nageleisen on 20/11/2014.
//  Copyright (c) 2014 lloeki. All rights reserved.
//

#ifndef xbox_one_controller_report_descriptor_h
#define xbox_one_controller_report_descriptor_h

static const unsigned char xbox_one_controller_report_descriptor[] = {
    0x05, 0x01,                     // Usage Page (Generic Desktop)
    0x09, 0x05,                     // Usage (Game Pad)
    0xa1, 0x01,                     // Collection (Application)

                                    //   # button packet
    0xa1, 0x00,                     //   Collection (Physical)
    0x85, 0x20,                     //     Report ID (0x20)

                                    //     # skip unknown field and counter
    0x05, 0x01,                     //     Usage Page (Generic Desktop)
    0x09, 0x00,                     //     Usage (Undefined)
    0x75, 0x08,                     //     Report Size (8)
    0x95, 0x02,                     //     Report Count (2)
    0x81, 0x03,                     //     Input (Constant, Variable, Absolute)

                                    //     payload size
    0x05, 0x01,                     //     Usage Page (Generic Desktop)
    0x09, 0x3b,                     //     Usage (Byte Count)
    0x75, 0x08,                     //     Report Size (8)
    0x95, 0x01,                     //     Report Count (1)
    0x81, 0x02,                     //     Input (Data, Variable, Absolute)

                                    //     # 16 buttons
    0x05, 0x09,                     //     Usage Page (Button)
    0x19, 0x01,                     //     Usage Minimum (Button 1)
    0x29, 0x10,                     //     Usage Maximum (Button 16)
    0x15, 0x00,                     //     Logical Minimum (0)
    0x25, 0x01,                     //     Logical Maximum (1)
    0x75, 0x01,                     //     Report Size (1)
    0x95, 0x10,                     //     Report Count (16)
    0x81, 0x02,                     //     Input (Data, Variable, Absolute)

                                    //     # triggers
    0x15, 0x00,                     //     Logical Minimum (0)
    0x26, 0xff, 0x03,               //     Logical Maximum (1023)
    0x35, 0x00,                     //     Physical Minimum (0)
    0x46, 0xff, 0x03,               //     Physical Maximum (1023)
    0x75, 0x10,                     //     Report Size (16)
    0x95, 0x02,                     //     Report Count (2)
    0x05, 0x01,                     //     Usage Page (Generic Desktop)
    0x09, 0x33,                     //     Usage (Rx)
    0x09, 0x34,                     //     Usage (Ry)
    0x81, 0x02,                     //     Input (Data, Variable, Absolute)

                                    //     # sticks
    0x75, 0x10,                     //     Report Size (16)
    0x16, 0x00, 0x80,               //     Logical Minimum (-32768)
    0x26, 0xff, 0x7f,               //     Logical Maximum (32767)
    0x36, 0x00, 0x80,               //     Physical Minimum (-32768)
    0x46, 0xff, 0x7f,               //     Physical Maximum (32767)
    0x05, 0x01,                     //     Usage Page (Generic Desktop)
    0x09, 0x01,                     //     Usage (Pointer)
    0xa1, 0x00,                     //     Collection (Physical)
    0x95, 0x02,                     //       Report Count (2)
    0x05, 0x01,                     //       Usage Page (Generic Desktop)
    0x09, 0x30,                     //       Usage (X)
    0x09, 0x31,                     //       Usage (Y)
    0x81, 0x02,                     //       Input (Data, Variable, Absolute)
    0xc0,                           //     End Collection
    0x05, 0x01,                     //     Usage Page (Generic Desktop)
    0x09, 0x01,                     //     Usage (Pointer)
    0xa1, 0x00,                     //     Collection (Physical)
    0x95, 0x02,                     //       Report Count (2)
    0x05, 0x01,                     //       Usage Page (Generic Desktop)
    0x09, 0x32,                     //       Usage (Z)
    0x09, 0x35,                     //       Usage (Rz)
    0x81, 0x02,                     //       Input (Data, Variable, Absolute)
    0xc0,                           //     End Collection
    0xc0,                           //   End Collection

                                    //   # xbox button packet
    0xa1, 0x00,                     //   Collection (Physical)
    0x85, 0x07,                     //     Report Id (0x07)

                                    //     # skip unknown field and counter
    0x05, 0x01,                     //     Usage Page (generic desktop)
    0x09, 0x00,                     //     Usage (Undefined)
    0x95, 0x02,                     //     Report Count (2)
    0x75, 0x08,                     //     Report Size (8)
    0x81, 0x03,                     //     Input (Constant, Variable, Absolute)

                                    //     # payload size
    0x05, 0x01,                     //     Usage Page (Generic Desktop)
    0x09, 0x3b,                     //     Usage (Byte Count)
    0x75, 0x08,                     //     Report Size (8)
    0x95, 0x01,                     //     Report Count (1)
    0x81, 0x03,                     //     Input (Constant, Variable, Absolute)

                                    //     # lone button
    0x05, 0x09,                     //     Usage Page (Button)
    0x09, 0x10,                     //     Usage (Button 17)
    0x15, 0x00,                     //     Logical Minimum (0)
    0x25, 0x01,                     //     Logical Maximum (1)
    0x75, 0x08,                     //     Report Size (8)
    0x95, 0x01,                     //     Report Count (1)
    0x81, 0x02,                     //     Input (Data, Variable, Absolute)

    0xc0,                           //   End Collection
    0xc0                            // End Collection
};

#endif
