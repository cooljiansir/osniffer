#include<pcap.h>
#include<stdio.h>
#include<stdlib.h> // for exit()
#include<string.h> //for memset
 
#include "envi.h"
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

//#define DEBUG
 
extern pcap_if_t *alldevs;
extern struct Interfaces *allinfs;

pthread_mutex_t mutex_cap;
int capon=0;
int filterset=0;

void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer);
void _SUCCESS_();
void _ERROR_();

void setfilter(char *filter_exp){
    struct Interfaces *pinf = allinfs;
    pcap_if_t *d = alldevs;
    char errbuf[100];
    while(pinf){
        if(pinf->selected==1){
		if(pinf->handle==NULL){
			pinf->handle = pcap_open_live(d->name, 65536 , pinf->pmode , 0 , errbuf);
		}
		if(pinf->handle==NULL)continue;
		if (pcap_compile(pinf->handle, &pinf->fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
			_ERROR_();
			return;
	 	}
		if (pcap_setfilter(pinf->handle, &pinf->fp) == -1) {
			_ERROR_();
			return;
		}	
        }
        pinf = pinf->next;
        d = d->next;
    }
    _SUCCESS_();
    filterset=1;
}
void * thread_loop(void *ptr){
	pcap_loop((pcap_t *)ptr,-1,process_packet,NULL);
	return NULL;
}
void opencap(){
    struct Interfaces *pinf = allinfs;
    pcap_if_t *d = alldevs;
    char errbuf[100]; 
    if(capon)return;
    capon = 1;
    //setfilter("not ( port 81 and host 192.168.1.1 )");
    //logfile = fopen("/tmp/log.txt","w");
    if(access(FIFO_CAP,F_OK)!=0){
	mkfifo(FIFO_CAP,0777);
    }
    pthread_mutex_init(&mutex_cap,NULL);
    while(pinf){
	if(pinf->selected==1){
    	    //pinf->handle = pcap_open_live(d->name, 65536 , pinf->pmode , 0 , errbuf);
	    if(pinf->handle==NULL){
		pinf->handle = pcap_open_live(d->name, 65536 , pinf->pmode , 0 , errbuf);
		if(filterset)pcap_setfilter(pinf->handle,&pinf->fp);
	    }
	    if(pinf->handle!=NULL){
		pthread_t t1;
		pthread_create(&t1,NULL,thread_loop,(void *)pinf->handle);
		//pcap_loop(pinf->handle,-1,process_packet,NULL);
	    }
	}
	pinf = pinf->next;
	d = d->next;
    }
}
void closecap(){
    struct Interfaces *pinf = allinfs;
    if(!capon)return;
    while(pinf){
	if(pinf->handle!=NULL){
	    pcap_close(pinf->handle);
	    pinf->handle = NULL;
	}
	pinf = pinf->next;
    }
    pthread_mutex_destroy(&mutex_cap);
    capon = 0;
}

char hextoch(int a){
    if(a>=0&&a<=9)return a+'0';
    else if(a>=10&&a<16)return a - 10 + 'A';
    return '.';
}

void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
    int size = header->len;
    int i;
    int ch;
    pthread_mutex_lock(&mutex_cap);
#ifdef DEBUG
    FILE *outfile = stdout;
#else
    FILE *outfile = fopen(FIFO_CAP,"w");
#endif
    if(outfile){
	fprintf(outfile,",{");
	fprintf(outfile,"\"time\":\"%d.%d\",\"data\":\"",header->ts.tv_sec,header->ts.tv_usec);
	for(i = 0;i<size;i++){
	    ch = buffer[i];
	    fprintf(outfile,"%c%c",hextoch(ch>>4),hextoch(ch&0xf));
	}
	fprintf(outfile,"\"}");
#ifdef DEBUG
#else
	fclose(outfile);	
#endif
    }
    pthread_mutex_unlock(&mutex_cap);
}
