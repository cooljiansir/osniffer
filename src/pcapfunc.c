#include <stdio.h>
#include <errno.h>

#include<sys/socket.h>
#include<arpa/inet.h> // for inet_ntoa()
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header

#include "envi.h"
#define READ_BUFF_SIZE 1024

char read_buff[READ_BUFF_SIZE];

void _ERROR_(){
	printf("file error");
}

void getlist(){
	FILE * pcmd = fopen(P_FIFO,"w");
	FILE * pout;
	if(pcmd==NULL){
		_ERROR_();
		return;
	}
	fputs(CMD_GETLIST,pcmd);
	fclose(pcmd);
	pcmd = NULL;
	pout = fopen(FIFO_OUT,"r");
	if(pout==NULL){
		_ERROR_();
		return;
	}
	while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
		printf("%s",read_buff);
	}

	fclose(pout);
}
void switch_select(char *name){
        FILE * pcmd = fopen(P_FIFO,"w");
        FILE * pout;
        if(pcmd==NULL){
                _ERROR_();
                return;
        }
        fputs(CMD_SET_SELECT,pcmd);
        fputs(" ",pcmd);
        fputs(name,pcmd);
        fclose(pcmd);
        pcmd = NULL;
        pout = fopen(FIFO_OUT,"r");
        if(pout==NULL){
                _ERROR_();
                return;
        }
        while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
                printf("%s",read_buff);
        }

        fclose(pout);

}
void switch_pmode(char *name){
        FILE * pcmd = fopen(P_FIFO,"w");
        FILE * pout;
        if(pcmd==NULL){
                _ERROR_();
                return;
        }
        fputs(CMD_SET_PMODE,pcmd);
	fputs(" ",pcmd);
	fputs(name,pcmd);
        fclose(pcmd);
        pcmd = NULL;
        pout = fopen(FIFO_OUT,"r");
        if(pout==NULL){
                _ERROR_();
                return;
        }
        while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
                printf("%s",read_buff);
        }

        fclose(pout);
}
void opencap(){
        FILE * pcmd = fopen(P_FIFO,"w");
        FILE * pout;
        if(pcmd==NULL){
                _ERROR_();
                return;
        }
        fputs(CMD_OPENCAP,pcmd);
        fclose(pcmd);
        pcmd = NULL;
        pout = fopen(FIFO_OUT,"r");
        if(pout==NULL){
                _ERROR_();
                return;
        }
        while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
                printf("%s",read_buff);
        }

        fclose(pout);
}
void closecap(){
        FILE * pcmd = fopen(P_FIFO,"w");
        FILE * pout;
        if(pcmd==NULL){
                _ERROR_();
                return;
        }
        fputs(CMD_CLOSECAP,pcmd);
        fclose(pcmd);
        pcmd = NULL;
        pout = fopen(FIFO_OUT,"r");
        if(pout==NULL){
                _ERROR_();
                return;
        }
        while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
                printf("%s",read_buff);
        }

        fclose(pout);
}
void getcap(){
        FILE * pout = fopen(FIFO_CAP,"r");
        if(pout==NULL){
                _ERROR_();
                if(pout)fclose(pout);
                return;
        }
	printf("[{}");
        while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
                printf("%s",read_buff);
        }
	printf("]");
        fclose(pout);
}
void setfilter(char *expr){
        FILE * pcmd = fopen(P_FIFO,"w");
        FILE * pout;
	int i;
        if(pcmd==NULL){
                _ERROR_();
                return;
        }
        fputs(CMD_SETFILTER,pcmd);
	fputs(" ",pcmd);
	//fputs(expr,pcmd);
	for(i = 0;expr[i];i++){
		if(expr[i]==' ')
			expr[i]='_';
	}
	fputs(expr,pcmd);
        fclose(pcmd);
        pcmd = NULL;
        pout = fopen(FIFO_OUT,"r");
        if(pout==NULL){
                _ERROR_();
                return;
        }
        while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
                printf("%s",read_buff);
        }

        fclose(pout);
}

