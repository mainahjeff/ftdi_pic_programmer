#if !defined(__FTCONF)
#define __FTCONF
#include <libusb-1.0/libusb.h>
#include "ftusb.h"
#include <ftd2xx.h>
#define _8BIT_1ST_NOP 0x0800
#define _8BIT_1ST_EP 0x0208
#define _8BIT_1ST_OP 0x0108
#define _8BIT_2ST_NOP 0x1008
#define _8BIT_2ST_EP 0x1208
#define _8BIT_2ST_OP 0x1108
///////////////////////
#define FLOW_RTS_CTS 0x0100
#define FLOW_DTR_DSR 0x0200
#define FLOW_NONE 0x0000
#define FLOW_XON_XOFF 0x0400
////////////////////////
#define BITMODE_RESET 0x0000
#define BITMODE_ASYNC_BITBANG 0x0100 // ft232r
#define BITMODE_MPSSE 0x0200
#define BITMODE_SYNC_BITBANG 0x0400 // ft232r
#define BITMODE_MCU_HOST 0x0800
#define BITMODE_FAST_SERIAL 0x1000
#define BITMODE_CBUS_BITBANG 0x2000 // ft232r
#define BITMODE_SYNC_FIFO 0x4000
/////////////////////////////////
#define FT_232R_CBUS_TXDEN 0x00      //	Tx Data Enable
#define FT_232R_CBUS_PWRON 0x01      //	Power On
#define FT_232R_CBUS_RXLED 0x02      //	Rx LED
#define FT_232R_CBUS_TXLED 0x03      //	Tx LED
#define FT_232R_CBUS_TXRXLED 0x04    //	Tx and Rx LED
#define FT_232R_CBUS_SLEEP 0x05      //	Sleep
#define FT_232R_CBUS_CLK48 0x06      //	48MHz clock
#define FT_232R_CBUS_CLK24 0x07      //	24MHz clock
#define FT_232R_CBUS_CLK12 0x08      //	12MHz clock
#define FT_232R_CBUS_CLK6 0x09       //	6MHz clock
#define FT_232R_CBUS_IOMODE 0x0A     //	IO Mode for CBUS bit-bang
#define FT_232R_CBUS_BITBANG_WR 0x0B //	Bit-bang write strobe
#define FT_232R_CBUS_BITBANG_RD 0x0C //	Bit-bang read strobe
#define FT_232R_CBUS0_RXF 0x0D       //	CBUS0 RXF#
#define FT_232R_CBUS1_TXE 0x0D       //	CBUS1 TXE#
#define FT_232R_CBUS2_RD 0x0D        //	CBUS2 RD#
#define FT_232R_CBUS3_WR 0x0D        //	CBUS3 WR#
DWORD bcnt = 0;
DWORD outputData;
char pindata[1];
uint16_t baud_cnt = 0;
void ftd_setBaud(libusb_device_handle *handle, long baud)
{
    switch (baud)
    {
    case 300:
        ftsend_cntr(handle, 0x40, 0x03, 0x2710, 0, 0, 0);
        break;
    case 600:
        ftsend_cntr(handle, 0x40, 0x03, 0x1388, 0, 0, 0);
        break;
    case 1200:
        ftsend_cntr(handle, 0x40, 0x03, 0x1388, 0, 0, 0);
        break;
    case 2400:
        ftsend_cntr(handle, 0x40, 0x03, 0x04E2, 0, 0, 0);
        break;
    case 4800:
        ftsend_cntr(handle, 0x40, 0x03, 0x0271, 0, 0, 0);
        break;
    case 9600:
        ftsend_cntr(handle, 0x40, 0x03, 0x4138, 0, 0, 0);
        break;
    case 19200:
        ftsend_cntr(handle, 0x40, 0x03, 0x809C, 0, 0, 0);
        break;
    case 38400:
        ftsend_cntr(handle, 0x40, 0x03, 0xC04E, 0, 0, 0);
        break;
    case 57600:
        ftsend_cntr(handle, 0x40, 0x03, 0x0034, 0, 0, 0);
        break;
    case 115200:
        ftsend_cntr(handle, 0x40, 0x03, 0x001A, 0, 0, 0);
        break;
    case 203400:
        ftsend_cntr(handle, 0x40, 0x03, 0x800E, 0, 0, 0);
        break;
    case 460800:
        ftsend_cntr(handle, 0x40, 0x03, 0x4006, 0, 0, 0);
        break;
    case 576000:
        ftsend_cntr(handle, 0x40, 0x03, 0x8005, 0, 0, 0);
        break;
    case 921600:
        ftsend_cntr(handle, 0x40, 0x03, 0x8003, 0, 0, 0);
        break;
    default:
        ftsend_cntr(handle, 0x40, 0x03, 0x4138, 0, 0, 0);
        printf("default");
        break;
    }
}
void ftd_setDataChar(libusb_device_handle *handle, int charx)
{
    switch (charx)
    {
    case _8BIT_1ST_NOP:
        ftsend_cntr(handle, 0x40, 0x03, charx, 0, 0, 0);
        break;
    case _8BIT_1ST_EP:
        ftsend_cntr(handle, 0x40, 0x03, charx, 0, 0, 0);
        break;
    case _8BIT_1ST_OP:
        ftsend_cntr(handle, 0x40, 0x03, charx, 0, 0, 0);
        break;
    case _8BIT_2ST_NOP:
        ftsend_cntr(handle, 0x40, 0x03, charx, 0, 0, 0);
        break;
    case _8BIT_2ST_EP:
        ftsend_cntr(handle, 0x40, 0x03, charx, 0, 0, 0);
        break;
    case _8BIT_2ST_OP:
        ftsend_cntr(handle, 0x40, 0x03, charx, 0, 0, 0);
        break;
    default:
        break;
    }
}
void ftd_open(libusb_device_handle *handle)
{
    ftsend_cntr(handle, 0x40, 0, 0, 0, 0, 0);
    for (int i = 0; i < 6; i++)
    {
        ftsend_cntr(handle, 0x40, 0, 1, 0, 0, 0);
    }
    ftsend_cntr(handle, 0x40, 0, 2, 0, 0, 0);
    // ftsend_cntr(handle, 0xC0, 0x05, 0, 0, 0, 2);
    // ftsend_cntr(handle, 0xC0, 10,0, 0, 0, 2);
}
void ftd_setRts(libusb_device_handle *handle)
{
    ftsend_cntr(handle, 0x40, 1, 0x0202, 0, 0, 0);
}
void ftd_setDtr(libusb_device_handle *handle)
{
    ftsend_cntr(handle, 0x40, 1, 0x0101, 0, 0, 0);
}
void ftd_set_pinDir(libusb_device_handle *handle, uint16_t bitmode, uint8_t ddr)
{
    ftsend_cntr(handle, 0x40, 11, bitmode | ddr, 0, 0, 0);
}
void ftd_bit_io(libusb_device_handle *handle, struct libusb_endpoint_descriptor endp, uint8_t pin)
{
    // ftd_set_pinDir(handle, BITMODE_ASYNC_BITBANG, 0xFF);

    int ftBulk = libusb_bulk_transfer(handle, endp.bEndpointAddress, getchar_2str(pin), 1, 0, 0);
    printf("ftBulk       %d  %s\n", ftBulk, libusb_error_name(ftBulk));
}
void ftd_reset_dev(libusb_device_handle *handle)
{
    ftsend_cntr(handle, 0x40, 0, 0, 0, 0, 0);
    for (int i = 0; i < 6; i++)
    {
        ftsend_cntr(handle, 0x40, 0, 1, 0, 0, 0);
    }
    ftsend_cntr(handle, 0x40, 0, 2, 0, 0, 0);
}
char ftd_pin_read(libusb_device_handle *handle)
{
    int ddr = ftsend_cntr(handle, 0xC0, 11, 0x01FF, 0, 0, 0) != 0; // INPUTS
    // printf("ddr     %d  %s\n", ddr, libusb_error_name(ddr));
    bzero(pindata, 1);
    int ftcntl = libusb_control_transfer(handle, 0xC0, 12, 0, 0, pindata, 1, 0);
    if (ftcntl != 0)
    {
        // printf("readpin     %d  %s\n", ftcntl, libusb_error_name(ftcntl));
        // printf("read pin error %d %x\n", ftcntl, pindata[0]);
        // exit(0);
    }
    //  = libusb_control_transfer(handle, 0xC0, 0x0C, 0, 0, pindata, 1, 0);

    return pindata[0];
}
void ftd_setFlow(libusb_device_handle *handle, uint16_t fcntr, char uxon, char uxoff)
{
    switch (fcntr)
    {
    case FLOW_RTS_CTS:
        ftsend_cntr(handle, 0x40, 2, 0x0100, 0, 0, 0);
        break;
    case FLOW_NONE:
        ftsend_cntr(handle, 0x40, 2, 0x0000, 0, 0, 0);
        break;
    case FLOW_XON_XOFF:
        ftsend_cntr(handle, 0x40, 2, (uxon << 8) | uxoff, 0, 0, 0);
        break;
    case FLOW_DTR_DSR:
        ftsend_cntr(handle, 0x40, 2, 0, 0X0200, 0, 0);
        ftsend_cntr(handle, 0x40, 1, 0x0101, 0, 0, 0);
        break;
    default:
        break;
    }
}
void ftd2_uart(libusb_device_handle *handle)
{
    ftd_set_pinDir(handle, BITMODE_RESET, 0);
}
/////////////////////////
uint8_t char2hex(unsigned char val)
{
    uint8_t u;
    if (!(isdigit(val)) == 0)
    {
        u = val - 0x30;
        return u;
    }
    else if (!(isxdigit(val)) == 0)
    {
        u = val - 0x37;
        return u;
    }
}
///////////////////////////////////
void ftd_send_char(FT_HANDLE ftHandle, unsigned char numchar)
{

    if (FT_Write(ftHandle, getchar_2str(numchar), 1, &outputData) != FT_OK)
    {
        printf("char write()  Error \n");
        exit(0);
    }
    // printf("%x\n", numchar);
}
void ftd_send_string(FT_HANDLE ftHandle, uint16_t del, char *word)
{
    unsigned char charnum[1024];
    bzero(charnum, 1024);
    uint8_t ccnt = 0;
    memcpy(charnum, word, strlen(word));
    for (uint8_t i = 0; i < strlen(charnum); i++)
    {
        ftd_send_char(ftHandle, char2hex(charnum[i]));
        printf("%c ", charnum[i]);
        // if (ccnt == 16)
        // {
        //     printf("\n");
        //     ccnt = 0;
        // }
        usleep((1000 * del));
        ccnt++;
    }
    printf("\n");
}
int ftd_file_read(FT_HANDLE ftHandle, uint16_t wr_del, uint16_t baud, uint16_t uslp, char filename[])
{
    unsigned char buff[2048];
    int fd;
    fd = open(filename, 'r', R_OK | W_OK | F_OK);
    int rd = read(fd, buff, sizeof(buff));
    // printf("size=       %d   %d\n", rd, cnts);
    // printf("strlen=     %ld\n", strlen(buff));
    // printf("size-buff=  %ld\n\n", sizeof(buff));

    if (FT_ResetDevice(ftHandle) != FT_OK)
        printf("reset() Error\n");
    if (FT_SetBaudRate(ftHandle, baud) != FT_OK)
        printf("Baud_set() Error\n");
    if (FT_SetTimeouts(ftHandle, wr_del, wr_del) != FT_OK)
        printf("timeout() Error");
    if (FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE) != FT_OK)
        printf("data chrx() Error\n");
    printf("...........Wait data On transit...............\n\n");
    ftd_send_string(ftHandle, uslp, buff);
    printf("\n");
    close(fd);
}
int ftd_read_stream(FT_HANDLE ftHandle, uint16_t baud, uint16_t delay)
{
    unsigned char mbuff[1];
    uint16_t buff_cnt = 0;
    if (FT_ResetDevice(ftHandle) != FT_OK)
        printf("reset() Error\n");
    if (FT_SetBaudRate(ftHandle, baud) != FT_OK)
        printf("Baud_set() Error\n");
    if (FT_SetTimeouts(ftHandle, delay, delay) != FT_OK)
        printf("timeout() Error");
    if (FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE) != FT_OK)
        printf("data chrx() Error\n");
    while (1)
    {
        bzero(mbuff, 64);
        // printf("read: ");
        // if (FT_Read(ftHandle, *ptr_buff_rxed, bytes_num2b_rd, *ptr_byte_num_rd) != FT_OK)

        if (FT_Read(ftHandle, &mbuff, 1, &outputData) != FT_OK)
        {
            printf("read() Error\n");
            exit(0);
        }

        printf("%.2X ", mbuff[0]);
        if (buff_cnt == 16)
        {
            printf("\n");
            buff_cnt = 0;
        }
        buff_cnt++;
    }
    printf("\n");
    // cnt++;
}
///////////////////////////////////
void ftd_bit_pins(FT_HANDLE ftHandle, uint8_t pin)
{
    DWORD outputData = pin;
    DWORD bytesWritten = 0;

    if (FT_SetBitMode(ftHandle, 0xFF, FT_BITMODE_ASYNC_BITBANG) != FT_OK)
    {
        printf("Bitmode() Error\n");
        exit(0);
    }
    ftd_send_char(ftHandle, pin);
    usleep(200);
}
uint8_t ftd_read_pins(FT_HANDLE ftHandle)
{
    unsigned char buff;
    unsigned char cnt = 0;
    if (FT_SetBitMode(ftHandle, 0XFF, FT_BITMODE_SYNC_BITBANG) != FT_OK)
        printf("BitMode() Error\n");
    if (FT_GetBitMode(ftHandle, &buff) != FT_OK)
        printf("getBitMode() Error\n");
    while ((buff >> 7) == 1)
    {
        // printf("BF: Error()\n");
        printf("BF: ERROR   %d \n", buff);
        exit(1);
    }
    // printf("%d ", buff);

    return buff;
}

#endif // __FTCONF