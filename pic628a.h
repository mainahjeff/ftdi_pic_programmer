#if !defined(__628A)
#define __628A
#include "icsp.h"

/////// COMMANDS ////////
#define bulk_er_prog 0x09    // 6ms
#define bulk_er_data 0x0B    // 6ms
#define ld_data_prog 0X02    // 2us
#define load_conf 0X00       // 2us
#define ld_data_mem 0X03     // 2us
#define increment 0X06       // 2us
#define rd_data_prog 0X04    // 2us
#define rd_data_mem 0X05     // 2us
#define begin_prog_only 0x08 // 4ms -6ms datamem
/////// DELAYS///////////
#define TSET0 100
#define THLD0 100
#define TSET1 100
#define THLD1 100
#define TDLY1 1000
#define TDLY2 1000
#define TDLY3 1000
#define TPROG 4000
#define TDPROG 6000
#define TERA 6000
#define TPPDP 5000
#define TLVPP 5000
#define TVHHR 1000

// ri dcd rsd dtr cts rts rx tx
// dcd rsd ri | rx |rts  |dtr  |tx
////////  pgm |vcc |mclr |clk |data   0x80,0x02,0x04,0x10,0x01
uint32_t p628a_comm_delays(uint8_t comm)
{
    switch (comm)
    {
    case bulk_er_prog:
        return TERA;
        break;
    case bulk_er_data:
        return TERA;
        break;
    case ld_data_prog:
        return TDLY2;
        break;
    case load_conf:
        return TDLY2;
        break;
    case ld_data_mem:
        return TDLY2;
        break;
    case increment:
        return TDLY2;
        break;
    case rd_data_prog:
        return TDLY2;
        break;
    case rd_data_mem:
        return TDLY2;
        break;
    case begin_prog_only:
        return TDPROG;
        break;
    default:
        return TERA;

        break;
    }
}
void p628_startH_V(FT_HANDLE ftHandle, uint32_t del) // S
{
    ftd_bits_i_o(ftHandle, ((0 & 0x1F)), del);              // vcc
    ftd_bits_i_o(ftHandle, ((0 & 0x1F) | vdd), del);        // vcc
    ftd_bits_i_o(ftHandle, ((0 & 0x1F) | vdd | mclr), del); // vcc +mclr
}
void p628_startL_V(FT_HANDLE ftHandle, uint32_t del) // S
{
    ftd_bits_i_o(ftHandle, ((0 & 0x1F)), del);
    ftd_bits_i_o(ftHandle, ((0 & 0x1F) | vdd), del);              // vcc
    ftd_bits_i_o(ftHandle, ((0 & 0x1F) | pgm | vdd), del);        // vcc
    ftd_bits_i_o(ftHandle, ((0 & 0x1F) | pgm | vdd | mclr), del); // vcc +mclr
}
void p628_stop(FT_HANDLE ftHandle, uint32_t del) // T
{
    ftd_bits_i_o(ftHandle, 0, del);                  // vcc
    ftd_bits_i_o(ftHandle, ((0 & 0x1F) | vdd), del); // vcc +mclr
}
void bulk_erase_H_V(FT_HANDLE ftHandle, uint32_t del) // K
{
    icsp_command(ftHandle, ld_data_prog, p628a_comm_delays(ld_data_prog));
    ftd16_hex2bit(ftHandle, 0x3FFF, del);
    icsp_command(ftHandle, bulk_er_prog, p628a_comm_delays(bulk_er_prog));
    icsp_command(ftHandle, load_conf, p628a_comm_delays(load_conf));
    icsp_command(ftHandle, bulk_er_prog, p628a_comm_delays(bulk_er_prog));
    icsp_command(ftHandle, bulk_er_data, p628a_comm_delays(bulk_er_data));
    p628_startH_V(ftHandle, del);
}
void bulk_erase_L_V(FT_HANDLE ftHandle, uint32_t del) // K
{
    ftd16_hex2bit(ftHandle, 0x3FFF, del);
    icsp_command(ftHandle, bulk_er_prog, p628a_comm_delays(bulk_er_prog));
    icsp_command(ftHandle, load_conf, p628a_comm_delays(load_conf));
    icsp_command(ftHandle, bulk_er_prog, p628a_comm_delays(bulk_er_prog));
    icsp_command(ftHandle, bulk_er_data, p628a_comm_delays(bulk_er_data));
    p628_startL_V(ftHandle, del);
}
void p628_increment(FT_HANDLE ftHandle) // I
{
    icsp_command(ftHandle, increment, p628a_comm_delays(increment));
}
///////////////////////
void p628_begProgm(FT_HANDLE ftHandle) // P
{
    icsp_command(ftHandle, begin_prog_only, p628a_comm_delays(begin_prog_only));
}
void p628_readprog(FT_HANDLE ftHandle) // R
{
    icsp16_read(ftHandle, rd_data_prog, p628a_comm_delays(rd_data_prog));
}
void p628_readconf(FT_HANDLE ftHandle) // Q
{
    icsp16_read(ftHandle, rd_data_prog, p628a_comm_delays(rd_data_prog));
}
void p628_prog_data(FT_HANDLE ftHandle) // M
{
    icsp_command(ftHandle, ld_data_prog, p628a_comm_delays(ld_data_prog));
}
void p628_conf_data(FT_HANDLE ftHandle) // N
{
    icsp_command(ftHandle, load_conf, p628a_comm_delays(load_conf));
}

#endif // __628A
