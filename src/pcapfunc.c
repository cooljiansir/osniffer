#include <stdio.h>
#include <errno.h>

#include "envi.h"
#define READ_BUFF_SIZE 1024

char read_buff[READ_BUFF_SIZE];

void _ERROR_(){
	printf("file error");
}

void getlist(){
	FILE * pcmd = fopen(P_FIFO,"w");
	FILE * pout = fopen(FIFO_OUT,"r");
	if(pcmd==NULL||pout==NULL){
		_ERROR_();
		if(pout)fclose(pout);
		if(pcmd)fclose(pcmd);
		return;
	}
	fputs(CMD_GETLIST,pcmd);
	fclose(pcmd);
	pcmd = NULL;
	
	while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
		printf("%s",read_buff);
	}

	fclose(pout);
}
void switch_select(char *name){
	FILE * pcmd = fopen(P_FIFO,"w");
        FILE * pout = fopen(FIFO_OUT,"r");
        if(pcmd==NULL||pout==NULL){
                _ERROR_();
                if(pout)fclose(pout);
                if(pcmd)fclose(pcmd);
                return;
        }
        fputs(CMD_SET_SELECT,pcmd);
	fputs(" ",pcmd);
	fputs(name,pcmd);
        fclose(pcmd);
        pcmd = NULL;

        while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
                printf("%s",read_buff);
        }

        fclose(pout);
}
void switch_pmode(char *name){
        FILE * pcmd = fopen(P_FIFO,"w");
        FILE * pout = fopen(FIFO_OUT,"r");
        if(pcmd==NULL||pout==NULL){
                _ERROR_();
                if(pout)fclose(pout);
                if(pcmd)fclose(pcmd);
                return;
        }
        fputs(CMD_SET_PMODE,pcmd);
        fputs(" ",pcmd);
        fputs(name,pcmd);
        fclose(pcmd);
        pcmd = NULL;

        while(fgets(read_buff,READ_BUFF_SIZE,pout)>0){
                printf("%s",read_buff);
        }
        fclose(pout);
}
