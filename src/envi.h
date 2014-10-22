#ifndef _ENVI_H__
#define _ENVI_H__

#include <pcap.h>

#define P_FIFO "/tmp/p_fifo"
#define FIFO_OUT "/tmp/p_out"
#define FIFO_CAP "/tmp/p_cap"

#define CMD_BUFF_SIZE 1024*10
//#define DEBUG

#define CMD_GETLIST "getlist"
#define CMD_SET_SELECT "setselect"      //setselect eth0 on
#define CMD_SET_PMODE "setpmode"        //setpmode eth0 on
#define CMD_GETCAP "getcap"
#define CMD_OPENCAP "opencap"
#define CMD_CLOSECAP "closecap"
#define CMD_SETFILTER "setfilter"


struct Interfaces{
        int selected;
        int packets;
        int pmode;
        pcap_t *handle;
	struct bpf_program fp;		/* The compiled filter expression */
        struct Interfaces *next;
};

#endif
