#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "ftusb.h"
#include "ftd_configs.h"
#include "lcd.h"
#include <ftd2xx.h>
char mbuff[1024];

extern unsigned char buffer[1024];
extern unsigned char Desc_buffer[1024];
unsigned char *int_buffer;
int *actLen;
uint16_t cnt = 0;

DWORD bytesWritten = 0;
DWORD outputData = 0;
DWORD inputData = 0;
int main(int argc, char *argv[])
{
    FT_HANDLE ftHandle = NULL;
    if (FT_Open(0, &ftHandle) != FT_OK)
    {
        printf("Open() Error\n");
        exit(0);
    }

    if (FT_ResetDevice(ftHandle) != FT_OK)
        printf("reset() Error\n");
    if (FT_SetBaudRate(ftHandle, 9600) != FT_OK)
        printf("Baud_set() Error\n");
    if (FT_SetTimeouts(ftHandle, 1300, 1300) != FT_OK)
        printf("timeout() Error");

    if (FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_2, FT_PARITY_NONE) != FT_OK)
        printf("data chrx() Error\n");
    if (FT_SetBitMode(ftHandle, 0xFF, FT_BITMODE_ASYNC_BITBANG) != FT_OK)
    {
        printf("Bitmode() Error\n");
        exit(0);
    }
    ftd_bit_pins(ftHandle, cts);
    usleep(10);
    lcd_command(ftHandle, 0, 140);

    lcd_init(ftHandle);

    command(ftHandle, 0X02, 1000);
    command(ftHandle, 0X07, 1000);
    // for (uint8_t i = 0; i < 8; i++)
    // {
    //     lcd_command(ftHandle, i, 8000);
    // }

    // lcd_command(ftHandle, 0X20, 24000);
    // lcd_command(ftHandle, 0X08, 1400);
    // lcd_command(ftHandle, 0x06, 40);

    // lcd_write(ftHandle, 'A', 200);
    // lcd_write(ftHandle, 'B', 200);
    // lcd_write(ftHandle, 'C', 200);
    // lcd_write(ftHandle, 'D', 200);
    // lcd_command(ftHandle, 0, 140);
    // for (uint8_t i = 0; i < 15; i++)
    // {
    // }

    FT_Close(ftHandle);
    printf("\nFTDI end\n");
}