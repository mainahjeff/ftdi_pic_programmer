#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <libusb-1.0/libusb.h>
#include <ftd2xx.h>
#include "ftusb.h"
#include "icsp.h"
#include "pic628a.h"

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
    // if (FT_ResetDevice(ftHandle) != FT_OK)
    //     printf("reset()or\n");
    // if (FT_SetBaudRate(ftHandle, 9600) != FT_OK)
    //     printf("Baud_set() Error\n");
    // if (FT_SetTimeouts(ftHandle, 100, 100) != FT_OK)
    //     printf("timeout() Error");
    // if (FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE) != FT_OK)
    //     printf("data chrx() Error\n");

    // ftd_freadFile(ftHandle, 100, "myfile.txt");
    // ftd_file_read(ftHandle, 100,"myfile.txt");
    // ftd_read_stream(ftHandle);
    // ftd_send_string(ftHandle, 80, "U0123456789ABCDEF");
    ftd_icsp_init(ftHandle, 0xFF);
    // 0xdtr-tx
    ftd6_hex2bit(ftHandle, 0x0A, 30000);
    ftd_stop(ftHandle, 0, 800);

    FT_Close(ftHandle);
    printf("FTDI end\n");
}