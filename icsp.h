#if !defined(__ICSP)
#define __ICSP

#include <libusb-1.0/libusb.h>
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

// ri dcd rsd dtr cts rts rx tx
// dcd rsd ri | rx |rts  |dtr  |tx
////////  pgm |vcc |mclr |clk |data   0x80,0x02,0x04,0x10,0x01

#define clk 0x10
#define bit 0x01
#define mclr 0x04
#define vdd 0x02
#define pgm 0x80
DWORD bit_cnt = 0;
DWORD outputData;
char pindata[1];
uint16_t baud_cnt = 0;

uint8_t hexbit_count = 0;
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
////////////////////       ////////////////////
void ftd_send_char(FT_HANDLE ftHandle, unsigned char numchar)
{
    if (FT_Write(ftHandle, getchar_2str(numchar), 1, &outputData) != FT_OK)
    {
        printf("char write()  Error \n");
        exit(0);
    }
    if (hexbit_count % 2)
    {
        // printf("%.2X ", (numchar & 0x01));
    }
    if (bit_cnt == 32)
    {
        // printf("\n");
        bit_cnt = 0;
    }
    hexbit_count++;
    bit_cnt++;
}

void ftd_send_string(FT_HANDLE ftHandle, uint16_t uslp, char *word)
{
    unsigned char charnum[1024];
    bzero(charnum, 1024);
    uint8_t ccnt = 0;
    memcpy(charnum, word, strlen(word));
    for (uint8_t i = 0; i < strlen(charnum); i++)
    {
        ftd_send_char(ftHandle, charnum[i]);
        // printf("%c ", charnum[i]);
        // if (ccnt == 16)
        // {
        //     printf("\n");
        //     ccnt = 0;
        // }
        usleep((1000 * uslp));
        ccnt++;
    }
    printf("\n");
}
int ftd_file_read(FT_HANDLE ftHandle, uint16_t uslp, char filename[])
{
    unsigned char buff[2048];
    int fd;
    fd = open(filename, 'r', R_OK | W_OK | F_OK);
    int rd = read(fd, buff, sizeof(buff));
    printf("rd; %d cnts: %d\n", rd, cnts);
    printf("strlen=     %ld\n", strlen(buff));
    printf("size-buff=  %ld\n\n", sizeof(buff));

    printf("...........Wait data On transit...............\n\n");
    ftd_send_string(ftHandle, uslp, buff);
    printf("\n");
    close(fd);
}
int ftd_freadFile(FT_HANDLE ftHandle, uint16_t uslp, char filename[])
{
    unsigned char buff[2048];
    FILE *fd;
    fd = fopen(filename, "r");
    int rd = fread(buff, sizeof(buff), strlen(buff), fd);
    printf("rd; %d cnts: %d\n", rd, cnts);
    printf("strlen=     %ld\n", strlen(buff));
    printf("size-buff=  %ld\n\n", sizeof(buff));

    printf("...........Wait data On transit...............\n\n");
    ftd_send_string(ftHandle, uslp, buff);
    printf("\n");
    fclose(fd);
}
int ftd_read_stream(FT_HANDLE ftHandle)
{
    unsigned char mbuff[1];
    uint16_t buff_cnt = 0;
    while (1)
    {
        bzero(mbuff, 1);
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
}
///////////////////////////////////
void ftd_icsp_init(FT_HANDLE ftHandle, uint8_t pindir) // 1-output 0-input
{
    if (FT_SetBitMode(ftHandle, pindir, FT_BITMODE_ASYNC_BITBANG) != FT_OK)
    {
        printf("Bitmode() Error\n");
        exit(0);
    }
}
void ftd_eusart_init(FT_HANDLE ftHandle, uint16_t baud, uint8_t readtm, uint8_t writetm)
{
    if (FT_Open(0, &ftHandle) != FT_OK)
    {
        printf("Open() Error\n");
        exit(0);
    }
    if (FT_ResetDevice(ftHandle) != FT_OK)
        printf("reset()or\n");
    if (FT_SetBaudRate(ftHandle, baud) != FT_OK)
        printf("Baud_set() Error\n");
    if (FT_SetTimeouts(ftHandle, readtm, writetm) != FT_OK)
        printf("timeout() Error");
    if (FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE) != FT_OK)
        printf("data chrx() Error\n");
}
void ftd_bits_i_o(FT_HANDLE ftHandle, uint8_t pin, uint32_t del)
{
    DWORD outputData = pin;
    // DWORD bytesWritten = 0;
    ftd_send_char(ftHandle, pin);
    usleep(del);
}
uint8_t ftd_read_pins(FT_HANDLE ftHandle)
{
    unsigned char buff;
    if (FT_GetBitMode(ftHandle, &buff) != FT_OK)
        printf("getBitMode() Error\n");
    printf("%.2x ", (buff & 0x01));
    return buff;
}
//////// ICSP /////////////////

uint8_t databuffer[16];
uint8_t commbuffer[8];
uint8_t comm_cnt = 0;
void ftd2icsp(FT_HANDLE ftHandle, uint8_t pin, uint32_t del)
{
    ftd_bits_i_o(ftHandle, ((clk & 0x1F) | vdd | mclr), del);
    ftd_bits_i_o(ftHandle, ((pin & 0x1F) | clk | vdd | mclr), del);
    ftd_bits_i_o(ftHandle, ((0 & 0x1F) | vdd | mclr), del);
}
void ftd16_hex2bit(FT_HANDLE ftHandle, uint16_t data, uint32_t del)
{
    uint16_t count = 0;

    uint16_t b = 2;
    div_t dd;
    dd = div(data, b);
    while (count < 16)
    {
        databuffer[count] = dd.rem;
        dd = div(dd.quot, b);
        count++;
    }
    ftd2icsp(ftHandle, 0, del);
    for (uint8_t i = 0; i <= 13; i++)
    {
        ftd2icsp(ftHandle, databuffer[i], del);
    }
    ftd2icsp(ftHandle, 0, del);
    for (uint8_t i = 0; i < 16; i++)
    {
        printf("%.2X ", (databuffer[i] & 0x01));
    }
    printf("\n");
}
void ftd6_hex2bit(FT_HANDLE ftHandle, uint8_t comm, uint32_t del)
{
    uint16_t count = 0;

    uint16_t b = 2;
    div_t dd;
    dd = div(comm, b);
    while (count < 6)
    {
        commbuffer[count] = dd.rem;
        dd = div(dd.quot, b);
        count++;
    }
    for (uint8_t i = 0; i < 6; i++)
    {
        ftd2icsp(ftHandle, commbuffer[i], del);
        // printf("%.2X ", (commbuffer[i] & 0x01));
        if (comm_cnt == 6)
        {
            // printf("\n");
            comm_cnt = 0;
        }
        comm_cnt++;
    }
}
void icsp_command(FT_HANDLE ftHandle, uint8_t comm, uint32_t del)
{
    ftd6_hex2bit(ftHandle, comm, del);
}
void icsp16_read(FT_HANDLE ftHandle, uint8_t comm, uint32_t comm_del)
{
    icsp_command(ftHandle, comm, comm_del);
    for (uint8_t i = 0; i < 16; i++)
    {
        ftd_bits_i_o(ftHandle, ((clk & 0x1F) | vdd | mclr), comm_del);
        ftd_bits_i_o(ftHandle, ((0 & 0x1F) | vdd | mclr), comm_del);
        ftd_read_pins(ftHandle);
    }
}
void icsp8_read(FT_HANDLE ftHandle, uint8_t comm, uint32_t comm_del)
{
    icsp_command(ftHandle, comm, comm_del);
    for (uint8_t i = 0; i < 8; i++)
    {
        ftd_bits_i_o(ftHandle, ((clk & 0x1F) | vdd | mclr), comm_del);
        ftd_bits_i_o(ftHandle, ((0 & 0x1F) | vdd | mclr), comm_del);
        ftd_read_pins(ftHandle);
    }
}

#endif // __ICSP
