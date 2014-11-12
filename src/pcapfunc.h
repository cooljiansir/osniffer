#ifndef _PCAPFUNC_H__
#define _PCAPFUNC_H__

//get the web interfaces list
void getlist();

//switch one interface on and off
void switch_select(char *name);

//switch on interface in promiscuous mode or not
void switch_pmode(char *name);

//get capture data
void getcap();

//set capture filter
void setfilter(char *expr);

//begin capture
void opencap();

//stop capture
void closecap();

//startserver
void startserver();

void _ERROR_();
void _SUCCESS_();

#endif
