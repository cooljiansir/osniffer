#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "envi.h"

//#define DEBUG


pcap_if_t *alldevs = NULL;
struct Interfaces *allinfs = NULL;


FILE *fcmd;
FILE *fout;
char cmdbuff[CMD_BUFF_SIZE];

void opencap();
void closecap();

void printpair(char *key,char *value){
	printf(",\"%s\":\"%s\"",key,value);
}
void printpaird(char *key,int num){
	printf(",\"%s\":\"%d\"",key,num);
}
void printpairfirst(char *key,char *value){
	printf("\"%s\":\"%s\"",key,value);
}

void freeInfs(){
	struct Interfaces * pinf=allinfs;
	struct Interfaces * pnext;
	while(pinf){
		pnext = pinf->next;
		free(pinf);
		pinf = pnext;
	}
	if(alldevs){
		pcap_freealldevs(alldevs);
	}
	alldevs = NULL;
	allinfs = NULL;
}
void freshlist(){
        pcap_if_t *d;
        char errbuff[PCAP_ERRBUF_SIZE];
	struct Interfaces *pinf=NULL;
	
	freeInfs();
	
        if(pcap_findalldevs(&alldevs,errbuff)==-1){
                //fprintf(stderr,"Error in pcap_findalldevs %s\n",errbuff);
		alldevs = NULL;
		allinfs = NULL;
                return;
        }
	for(d=alldevs;d;d=d->next){
		if(pinf==NULL){
			allinfs = pinf = (struct Interfaces *)malloc(sizeof(struct Interfaces));
		}else{
			pinf->next = (struct Interfaces *)malloc(sizeof(struct Interfaces));
			pinf = pinf->next;
		}
		pinf->next = NULL;
		pinf->selected = 0;
		pinf->packets = 0;
		pinf->pmode = 1;
		pinf->handle = NULL;
	}
}
void getlist(){
        pcap_if_t *d;
        pcap_addr_t *paddr;
	struct Interfaces *pinf;
        char straddr[INET6_ADDRSTRLEN];
        int i=0;
        char errbuff[PCAP_ERRBUF_SIZE];
	int countdevs = 0;
	int countips;

	if(alldevs==NULL){
		freshlist();
		if(alldevs==NULL)
			return;
	}

	printf("[");
        for(d=alldevs,pinf=allinfs;d;d=d->next,pinf=pinf->next){
		countdevs++;
		if(countdevs>1)printf(",");
		printf("{");
		printpairfirst("name",d->name);
		printpaird("selected",pinf->selected);
		printpaird("packes",pinf->packets);
		printpaird("pmode",pinf->pmode);
                if(d->description)
			printpair("description",d->description);
		else printpair("description","none");
                paddr = d->addresses;
		printf(",\"ips\":[");
		countips=0;
                while(paddr){
                        if(paddr->addr->sa_family == AF_INET||paddr->addr->sa_family == AF_INET6){
				countips++;
				if(countips>1)printf(",{");
				else printf("{");
	                        if(paddr->addr->sa_family == AF_INET6){
        	                        inet_ntop(AF_INET6,&((struct sockaddr_in6 *)paddr->addr)->sin6_addr, straddr,sizeof(straddr));
                        	       	printpairfirst("ip",straddr);
                        	}
				else 
					printpairfirst("ip",inet_ntoa(((struct sockaddr_in*)paddr->addr)->sin_addr));
				printf("}");
			}
                        paddr = paddr->next;
                }
		printf("]");
                printf("}");
        }
	printf("]");
}
void _ERROR_(){
	printf("{\"result\":\"error\"}");
}
void _SUCCESS_(){
	printf("{\"result\":\"success\"}");
}
void serveone(){
	//最多5个参数
	char param[5][1024];
	int params;
	
	pcap_if_t *d;
	struct Interfaces *pinf;
	
	if(access(P_FIFO,F_OK)!=0){
		if(mkfifo(P_FIFO,0777)<0)
			return;
	}
	if(access(FIFO_OUT,F_OK)!=0){
		if(mkfifo(FIFO_OUT,0777)<0)
			return;
	}
#ifdef DEBUG
	fcmd = stdin;
#else
	fcmd = fopen(P_FIFO,"r");
	freopen(FIFO_OUT,"w",stdout);
#endif

	//如果写端关闭了连接,会退出
	while(fgets(cmdbuff,CMD_BUFF_SIZE,fcmd)>0){
		params = sscanf(cmdbuff,"%s %s %s %s %s",param[0],param[1],param[2],param[3],param[4]);
		if(strcmp(param[0],CMD_GETLIST)==0){
			getlist();
		}
		else if(strcmp(param[0],CMD_SET_SELECT)==0){
			if(params!=2){
				_ERROR_();
			}else{
				int res = 0;
				for(d=alldevs,pinf = allinfs;d;d=d->next,pinf=pinf->next){
					if(strcmp(d->name,param[1])==0){
						pinf->selected = !pinf->selected;
						res = 1;
					}
				}
				if(!res)_ERROR_();
				else _SUCCESS_();
			}
		}else if(strcmp(param[0],CMD_SET_PMODE)==0){
                        if(params!=2){
                                _ERROR_();
                        }else{
                                int res = 0;
                                for(d=alldevs,pinf = allinfs;d;d=d->next,pinf=pinf->next){
                                        if(strcmp(d->name,param[1])==0){
                                        	res = 1;
                                                pinf->pmode = !pinf->pmode;
                                        }
                                }
                                if(!res)_ERROR_();
				else _SUCCESS_();
                        }
		}else if(strcmp(param[0],CMD_OPENCAP)==0){
			opencap();
		}else if(strcmp(param[0],CMD_CLOSECAP)==0){
			closecap();
		}
	}
	fclose(fcmd);
	fclose(stdout);
}

void doserve(){
	while(1){
		serveone();
	}
}

int main(){
	doserve();
	return 0;
}
