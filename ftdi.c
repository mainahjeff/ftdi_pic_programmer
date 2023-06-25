#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <libusb-1.0/libusb.h>
#include "ftusb.h"
#include "ftd_configs.h"
#include "lcd.h"
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
    LONG comport=0;
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

    // ftd_file_read(ftHandle, 300, 9600,20 ,"myfile.txt");
    ftd_read_stream(ftHandle, 9600, 60);
    // ftd_send_string(ftHandle,80,"U0123456789ABCDEF");

    // if (FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0,0 ) != FT_OK)
    //     printf("Flow() Error\n");
    // if (FT_SetFlowControl(ftHandle, FT_FLOW_DTR_DSR, 0,0 ) != FT_OK)
    //     printf("Flow() Error\n");
    // if (FT_SetFlowControl(ftHandle, FT_FLOW_NONE,UCHAR,UCHAR) != FT_OK)
    //     printf("Flow() Error\n");
    // FT_SetRts(ftHandle); // off
    // FT_ClrRts(ftHandle);//on
    // FT_SetDtr(ftHandle); // off
    // FT_ClrDtr(ftHandle);//on

    FT_Close(ftHandle);
    printf("FTDI end\n");
}