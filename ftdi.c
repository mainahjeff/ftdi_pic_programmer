#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <libusb-1.0/libusb.h>
#include "ftusb.h"
#include "ftdi.h"
#include <ftd2xx.h>
unsigned char mbuff[1024];

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
    LONG comport = 0;
    FT_HANDLE ftHandle = NULL;
    if (FT_Open(0, &ftHandle) != FT_OK)
    {
        printf("Open() Error\n");
        exit(0);
    }
    if (FT_ResetDevice(ftHandle) != FT_OK)
        printf("reset()or\n");
    if (FT_SetBaudRate(ftHandle, 9600) != FT_OK)
        printf("Baud_set() Error\n");
    if (FT_SetTimeouts(ftHandle, 100, 100) != FT_OK)
        printf("timeout() Error");
    if (FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE) != FT_OK)
        printf("data chrx() Error\n");

    // ftd_freadFile(ftHandle, 100, "myfile.txt");
    // ftd_file_read(ftHandle, 100,"myfile.txt");
    // ftd_read_stream(ftHandle);
    // ftd_send_string(ftHandle, 80, "U0123456789ABCDEF");
    ftd_icsp_init(ftHandle, 0xFF);
    // 0xdtr-tx
    uint8_t test[10] = {0, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01};
    // ri dcd rsd dtr cts rts rx tx
    // dcd rsd ri | rx |rts  |dtr  |tx   2,4,16,1
    ///////////// |vcc |mclr |data |clk
    for (int i = 0; i < 10; i++)
    {
        ftd_bit_pins(ftHandle, ((test[i] & 0x1F) | 0X14), 800000);
    }
    ftd_bit_pins(ftHandle, 0, 800);

    FT_Close(ftHandle);
    printf("FTDI end\n");
}