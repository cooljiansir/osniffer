#ifndef _ENVI_H__
#define _ENVI_H__

#include <pcap.h>

#define P_FIFO "/tmp/p_fifo"
#define FIFO_OUT "/tmp/p_out"
#define FIFO_CAP "/tmp/p_cap"
#define FILE_LCK "/tmp/lock_osniffer"

#define CMD_BUFF_SIZE 1024*10

#define CMD_GETLIST "getlist"
#define CMD_SET_SELECT "setselect"      //setselect eth0 on
#define CMD_SET_PMODE "setpmode"        //setpmode eth0 on
#define CMD_GETCAP "getcap"
#define CMD_OPENCAP "opencap"
#define CMD_CLOSECAP "closecap"
#define CMD_SETFILTER "setfilter"

#define PATH_OSNIFFERD "/webroot/cgi-bin/osnifferd"

#endif
