#include <libusb-1.0/libusb.h>
#include <ftd2xx.h>
#include "ftusb.h"
#include "icsp.h"
#include "pic628a.h"

int main(int argc, char *argv[])
{
    LONG comport = 0;
    FT_HANDLE ftHandle = NULL;
    if (FT_Open(0, &ftHandle) != FT_OK)
    {
        printf("Open() Error\n");
        exit(0);
    }
    ftd_icsp_init(ftHandle, 0xFF);
    



    
    FT_Close(ftHandle);
    printf("\nFTDI end\n");
}
