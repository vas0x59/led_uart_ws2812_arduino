
#pragma once

#include <Arduino.h>
#include <stdint.h>
// #include <string.h>
#include "checksum.h"
namespace serial_led
{
    
using namespace std;

#define STX 0xFE
#define END 0xFA

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    String ToString();
};

struct Message
{
    uint8_t type;
    uint8_t p1;
    uint8_t p2;
    uint8_t p3;
    uint8_t p4;
    uint8_t p5;
    // uint8_t c//
    // uint8_t msg[4];
};

enum
{
    CMD_FILL_COLOR = 1,
    CMD_SET_PIXEL_COLOR
};

void message_encode_fill_color(uint8_t led_count, Color color, Message &msg);
void message_decode_fill_color(uint8_t &led_count, Color &color, Message msg);

void message_encode_set_pixel_color(uint8_t led_count, uint8_t index, Color color, Message &msg);
void message_decode_set_pixel_color(uint8_t &led_count, uint8_t &index, Color &color, Message msg);

void message_to_send_buf(Message msg, uint8_t *buf);
bool message_from_buf(Message &msg, uint8_t *buf);

// union Message {

// }

}