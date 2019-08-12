#include "message.h"

namespace serial_led
{

using namespace std;

#define STX 0xFE
#define END 0xFA

String Color::ToString()
{
    return String(r) + ' ' + String(g) + ' ' + String(b);
}

void message_encode_fill_color(uint8_t led_count, Color color, Message &msg)
{
    msg.type = CMD_FILL_COLOR;
    msg.p1 = led_count;
    msg.p2 = color.r;
    msg.p3 = color.g;
    msg.p4 = color.b;
    msg.p5 = 0;
}

void message_decode_fill_color(uint8_t &led_count, Color &color, Message msg)
{
    led_count = msg.p1;
    color.r = msg.p2;
    color.g = msg.p3;
    color.b = msg.p4;
}

void message_encode_set_pixel_color(uint8_t led_count, uint8_t index, Color color, Message &msg)
{
    msg.type = CMD_SET_PIXEL_COLOR;
    msg.p1 = led_count;
    msg.p2 = color.r;
    msg.p3 = color.g;
    msg.p4 = color.b;
    msg.p5 = index;
}

void message_decode_set_pixel_color(uint8_t &led_count, uint8_t &index, Color &color, Message msg)
{
    led_count = msg.p1;
    color.r = msg.p2;
    color.g = msg.p3;
    color.b = msg.p4;
    index = msg.p5;
}

void message_to_send_buf(Message msg, uint8_t *buf)
{
    buf[0] = STX;
    // uint8_t payload[5];
    buf[1] = msg.type;
    buf[2] = msg.p1;
    buf[3] = msg.p2;
    buf[4] = msg.p3;
    buf[5] = msg.p4;
    buf[6] = msg.p5;
    // buf[1] =
    uint16_t checksum = crc_calculate(buf, 7);
    buf[7] = (uint8_t)(checksum & 0xFF);
    buf[8] = (uint8_t)(checksum >> 8);
    buf[9] = END;
    // crc_accumulate_buffer(&checksum, (char *)payload, sizeof(payload));
    // crc_accumulate(crc_extra, &checksum);
    // mavlink_ck_a(msg) = (uint8_t)(checksum & 0xFF);
    // mavlink_ck_b(msg) = (uint8_t)(checksum >> 8);

    // msg->checksum = checksum;
}

bool message_from_buf(Message &msg, uint8_t *buf)
{
    // buf[0
    if ((buf[0] == STX) && (buf[9] == END))
    {
        // cout << "STX ok\n";
        uint16_t checksum = crc_calculate(buf, 7);
        if ((buf[7] == (uint8_t)(checksum & 0xFF)) && (buf[8] == (uint8_t)(checksum >> 8)))
        {
            // cout << "checksum ok\n";
            if ((buf[1] > 0) && (buf[1] <= CMD_SET_PIXEL_COLOR))
            {
                // cout << "cmd ok\n";
                msg.type = buf[1];
                msg.p1 = buf[2];
                msg.p2 = buf[3];
                msg.p3 = buf[4];
                msg.p4 = buf[5];
                msg.p5 = buf[6];
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}
} // namespace serial_led