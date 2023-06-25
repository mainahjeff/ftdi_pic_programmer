#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include "ftd_configs.h"
#if !defined(__LCD)
#define __LCD
// tx rx rts  cts dtr rsd dcd r1
// rs rw  en       d4  d5  d6  d7
// 01 02  04       10  20  40  80
#define rs 0x01
#define rw 0x02
#define en 0x04
#define cts 0x08

#define func_1line 0x00
#define func_2line 0x08
#define func_5x8 0x00
#define func_5x11 0x04
////////////
#define disp_off 0x08
#define clr_ret_hm 0x01
///////////////////
#define inc_cursor_right 0x06
uint8_t status = 0;
uint8_t bitcnt = 0;
//////////////////
uint8_t read_flag(FT_HANDLE handle, uint16_t del)
{
    unsigned char buff;
    ftd_bit_pins(handle, en | rw);
    if (FT_GetBitMode(handle, &buff) != FT_OK)
        printf("getBitMode() Error\n");
    while ((buff >> 7) == 1)
    {
        // printf("BF: Error()\n");
        printf("BF: ERROR   %d \n", buff);
        exit(1);
    }
    printf("buff: %d \n", buff);

    usleep((del * 100));
    return buff;
}
void command(FT_HANDLE handle, uint8_t data, uint16_t del)
{
    usleep((del * 20));
    uint8_t lsb = ((data & 0x0F) << 4);
    uint8_t msb = (data & 0XF0);
    ftd_bit_pins(handle, en);
    ftd_bit_pins(handle, en | lsb);
    ftd_bit_pins(handle, msb);
    ftd_bit_pins(handle, en | msb);
    usleep((del * 100));
    // ftd_bit_pins(handle, 0);
}
void lcd_command(FT_HANDLE handle, uint8_t data, uint16_t del)
{
    uint8_t lsb = ((data & 0x0F) << 4);
    uint8_t msb = (data & 0XF0);
    ftd_bit_pins(handle, msb);
    ftd_bit_pins(handle, en | msb);    
    ftd_bit_pins(handle, lsb);
    ftd_bit_pins(handle, en | lsb);
    ftd_bit_pins(handle, 0);
    usleep((del * 1000));
    read_flag(handle, 1400);
}
void lcd_write(FT_HANDLE handle, uint8_t data, uint16_t del)
{
    uint8_t lsb = ((data & 0x0F) << 4);
    uint8_t msb = (data & 0XF0);
    ftd_bit_pins(handle, lsb);
    ftd_bit_pins(handle, en | rs | lsb);
    ftd_bit_pins(handle, msb);
    ftd_bit_pins(handle, en | rs | msb);
    ftd_bit_pins(handle, 0);
    usleep((del * 1000));
}

void lcd_init(FT_HANDLE handle)
{
    command(handle, 0x30, 2000);
    command(handle, 0x30, 500);
    command(handle, 0x30, 240);
    command(handle, 0, 140);

}

#endif // __LCD
