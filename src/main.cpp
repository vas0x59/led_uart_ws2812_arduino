#include <Arduino.h>
// #include "checksum.h"
#include "message.h"
#include <Adafruit_NeoPixel.h>

using namespace serial_led;


Adafruit_NeoPixel *pixels;

int pin =  5;

bool first_cmd = true;
uint8_t led_count;

// union Colo
// {
//     /* data */
// };


void fill_color(Color color){
    for (uint8_t i = 0; i < led_count; i++)
    {
        // pixels->fill()
        pixels->setPixelColor(i, color.r, color.g, color.b);
    }
    pixels->show();
}

void set_pixel_color(uint8_t index, Color color){
    pixels->setPixelColor(index, color.r, color.g, color.b);
    pixels->show();
}

void handle_message(Message msg_r)
{
    if (first_cmd == true){
        first_cmd = false;
        led_count = msg_r.p1;
        pixels = new Adafruit_NeoPixel(led_count, pin, NEO_BGR + NEO_KHZ800);
        pixels->begin();
    }
    switch (msg_r.type)
    {
    case CMD_FILL_COLOR:
    {
        Color color;
        message_decode_fill_color(led_count, color, msg_r);
        fill_color(color);
    }
    break;
    case CMD_SET_PIXEL_COLOR:
    {
        Color color;
        uint8_t index;
        message_decode_set_pixel_color(led_count, index, color, msg_r);
        set_pixel_color(index, color);
    }
    break;
    default:
        break;
    }
}

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available() > 0)
    {
        Message msg;
        uint8_t buf[10];
        // for (uint8_t i = 0; i < 10; i++)
        // {
        //     buf[i]  = Serial.read
        // }
        Serial.readBytes(buf, 10);
        
        bool success = message_from_buf(msg, buf);
        if (success)
        {
            handle_message(msg);
        }
        else
        {
        }
    }
}