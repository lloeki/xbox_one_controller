//
//  xbox_one_controller_packet.h
//  xbox_one_controller
//
//  Created by Loic Nageleisen on 06/02/15.
//  Copyright (c) 2015 lloeki. All rights reserved.
//

#ifndef xbox_one_controller_xbox_one_controller_packet_h
#define xbox_one_controller_xbox_one_controller_packet_h

enum xbox_one_controller_packet_type {
    xbox_one_controller_packet_type_btn = 0x20
};

typedef struct xbox_one_controller_packet {
    uint8_t  type; // packet type
    uint8_t  unk;  // always 0x00?
    uint8_t  seq;  // counter [0x00-0xff]
    uint8_t  len;  // payload length (0x0e for type 0x20)
    uint16_t btn;  // bitmask
    uint16_t lt;   // LE [ 0x0000:0x03ff]
    uint16_t rt;   // LE [ 0x0000:0x03ff]
    int16_t  lx;   // LE [-0x8000:0x7fff]
    int16_t  ly;   // LE [-0x8000:0x7fff]
    int16_t  rx;   // LE [-0x8000:0x7fff]
    int16_t  ry;   // LE [-0x8000:0x7fff]
} xbox_one_controller_packet;

#endif
