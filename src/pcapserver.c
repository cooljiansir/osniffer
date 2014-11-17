#include <pcap.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <pcap.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "envi.h"
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include "pcapserver.h"

/*
 * this file implements all the functions 
 * declared in pcapserver.h
 * notice:all the output should be in json format
 */


//all the result output to this file
extern FILE *fout;

//some global variable needed in this file
pcap_if_t *alldevs = NULL;
struct Interfaces *allinfs = NULL;
pthread_mutex_t mutex_cap;
int capon = 0;
int filterset = 0;

void printpair(char *key, char *value) {
	fprintf(fout, ",\"%s\":\"%s\"", key, value);
}
void printpaird(char *key, int num) {
	fprintf(fout, ",\"%s\":\"%d\"", key, num);
}
void printpairfirst(char *key, char *value) {
	fprintf(fout, "\"%s\":\"%s\"", key, value);
}
void _ERROR_() {
	fprintf(fout, "{\"result\":\"error\"}");
}
void _SUCCESS_() {
	fprintf(fout, "{\"result\":\"success\"}");
}

void setfilter(char *filter_exp) {
	struct Interfaces *pinf = allinfs;
	pcap_if_t *d = alldevs;
	char errbuf[100];
	while (pinf) {
		if (pinf->selected == 1) {
			if (pinf->handle == NULL) {
				pinf->handle = pcap_open_live(d->name, 65536, pinf->pmode, 0,
						errbuf);
			}
			if (pinf->handle == NULL)
				continue;
			if (pcap_compile(pinf->handle, &pinf->fp, filter_exp, 0,
					PCAP_NETMASK_UNKNOWN) == -1) {
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
	filterset = 1;
}

void process_packet(u_char *args, const struct pcap_pkthdr *header,
		const u_char *buffer);
void * thread_loop(void *ptr) {
	pcap_loop((pcap_t *) ptr, -1, process_packet, NULL);
	return NULL;
}
void opencap() {
	struct Interfaces *pinf = allinfs;
	pcap_if_t *d = alldevs;
	char errbuf[100];
	if (capon)
		return;
	capon = 1;
	if (access(FIFO_CAP, F_OK) != 0) {
		mkfifo(FIFO_CAP, 0777);
	}
	pthread_mutex_init(&mutex_cap, NULL);
	while (pinf) {
		if (pinf->selected == 1) {
			if (pinf->handle == NULL) {
				pinf->handle = pcap_open_live(d->name, 65536, pinf->pmode, 0,
						errbuf);
				if (filterset)
					pcap_setfilter(pinf->handle, &pinf->fp);
			}
			if (pinf->handle != NULL) {
				pthread_t t1;
				pthread_create(&t1, NULL, thread_loop, (void *) pinf->handle);
				//pcap_loop(pinf->handle,-1,process_packet,NULL);
			}
		}
		pinf = pinf->next;
		d = d->next;
	}
}
void closecap() {
	struct Interfaces *pinf = allinfs;
	if (!capon)
		return;
	while (pinf) {
		if (pinf->handle != NULL) {
			pcap_close(pinf->handle);
			pinf->handle = NULL;
		}
		pinf = pinf->next;
	}
	pthread_mutex_destroy(&mutex_cap);
	capon = 0;
}

char hextoch(int a) {
	if (a >= 0 && a <= 9)
		return a + '0';
	else if (a >= 10 && a < 16)
		return a - 10 + 'A';
	return '.';
}

void process_packet(u_char *args, const struct pcap_pkthdr *header,
		const u_char *buffer) {
	int size = header->len;
	int i;
	int ch;
	pthread_mutex_lock(&mutex_cap);
#ifdef DEBUG
	FILE *outfile = stdout;
#else
	FILE *outfile = fopen(FIFO_CAP, "w");
#endif
	if (outfile) {
		fprintf(outfile, ",{");
		fprintf(outfile, "\"time\":\"%d.%d\",\"data\":\"", header->ts.tv_sec,
				header->ts.tv_usec);
		for (i = 0; i < size; i++) {
			ch = buffer[i];
			fprintf(outfile, "%c%c", hextoch(ch >> 4), hextoch(ch & 0xf));
		}
		fprintf(outfile, "\"}");
#ifdef DEBUG
#else
		fclose(outfile);
#endif
	}
	pthread_mutex_unlock(&mutex_cap);
}

void freeInfs() {
	struct Interfaces * pinf = allinfs;
	struct Interfaces * pnext;
	while (pinf) {
		pnext = pinf->next;
		free(pinf);
		pinf = pnext;
	}
	if (alldevs) {
		pcap_freealldevs(alldevs);
	}
	alldevs = NULL;
	allinfs = NULL;
}
void freshlist() {
	pcap_if_t *d;
	char errbuff[PCAP_ERRBUF_SIZE];
	struct Interfaces *pinf = NULL;

	freeInfs();

	if (pcap_findalldevs(&alldevs, errbuff) == -1) {
		//fprintf(stderr,"Error in pcap_findalldevs %s\n",errbuff);
		alldevs = NULL;
		allinfs = NULL;
		return;
	}
	for (d = alldevs; d; d = d->next) {
		if (pinf == NULL) {
			allinfs = pinf = (struct Interfaces *) malloc(
					sizeof(struct Interfaces));
		} else {
			pinf->next = (struct Interfaces *) malloc(
					sizeof(struct Interfaces));
			pinf = pinf->next;
		}
		pinf->next = NULL;
		pinf->selected = 0;
		pinf->packets = 0;
		pinf->pmode = 1;
		pinf->handle = NULL;
	}
}
void getlist() {
	pcap_if_t *d;
	pcap_addr_t *paddr;
	struct Interfaces *pinf;
	char straddr[INET6_ADDRSTRLEN];
	int i = 0;
	char errbuff[PCAP_ERRBUF_SIZE];
	int countdevs = 0;
	int countips;

	if (alldevs == NULL) {
		freshlist();
		if (alldevs == NULL)
			return;
	}

	fprintf(fout, "[");
	for (d = alldevs, pinf = allinfs; d; d = d->next, pinf = pinf->next) {
		countdevs++;
		if (countdevs > 1)
			fprintf(fout, ",");
		fprintf(fout, "{");
		printpairfirst("name", d->name);
		printpaird("selected", pinf->selected);
		printpaird("packes", pinf->packets);
		printpaird("pmode", pinf->pmode);
		if (d->description)
			printpair("description", d->description);
		else
			printpair("description", "none");
		paddr = d->addresses;
		fprintf(fout, ",\"ips\":[");
		countips = 0;
		while (paddr) {
			if (paddr->addr->sa_family == AF_INET
					|| paddr->addr->sa_family == AF_INET6) {
				countips++;
				if (countips > 1)
					fprintf(fout, ",{");
				else
					fprintf(fout, "{");
				if (paddr->addr->sa_family == AF_INET6) {
					inet_ntop(AF_INET6,
							&((struct sockaddr_in6 *) paddr->addr)->sin6_addr,
							straddr, sizeof(straddr));
					printpairfirst("ip", straddr);
				} else
					printpairfirst("ip",
							inet_ntoa(
									((struct sockaddr_in*) paddr->addr)->sin_addr));
				fprintf(fout, "}");
			}
			paddr = paddr->next;
		}
		fprintf(fout, "]");
		fprintf(fout, "}");
	}
	fprintf(fout, "]");
}
void setselect(char *name) {
	pcap_if_t *d;
	struct Interfaces *pinf;
	int res = 0;
	for (d = alldevs, pinf = allinfs; d; d = d->next, pinf = pinf->next) {
		if (strcmp(d->name, name) == 0) {
			pinf->selected = !pinf->selected;
			res = 1;
		}
	}
	if (!res)
		_ERROR_();
	else
		_SUCCESS_();
}

void setpmode(char *name) {
	pcap_if_t *d;
	struct Interfaces *pinf;
	int res = 0;
	for (d = alldevs, pinf = allinfs; d; d = d->next, pinf = pinf->next) {
		if (strcmp(d->name, name) == 0) {
			res = 1;
			pinf->pmode = !pinf->pmode;
		}
	}
	if (!res)
		_ERROR_();
	else
		_SUCCESS_();
}
