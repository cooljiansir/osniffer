#ifndef _PCAP_SERVER_H_
#define _PCAP_SERVER_H_

//get the interfaces list
void getlist();

//switch one interface on and off
void setselect(char *);

//switch on interface in promiscuous mode or not
void setpmode(char *);

//set capture filter
void setfilter(char *);

//begin capture
void opencap();

//stop capture
void closecap();

struct Interfaces {
	int selected;
	int packets;
	int pmode;
	pcap_t *handle;
	struct bpf_program fp; /* The compiled filter expression */
	struct Interfaces *next;
};

#endif
