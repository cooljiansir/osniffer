#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "envi.h"
#include "pcapserver.h"

#define DEBUG

/*
 * this is the main requests router
 * deals all the requests from pcapfunc.c
 * call functions in pcapserver.h
 */

FILE *fcmd;
FILE *fout;
char cmdbuff[CMD_BUFF_SIZE];

void serveone() {
	//at most 5 params
	char param[5][1024];
	int params;

	if (access(P_FIFO, F_OK) != 0) {
		if (mkfifo(P_FIFO, 0777) < 0)
			return;
	}
	if (access(FIFO_OUT, F_OK) != 0) {
		if (mkfifo(FIFO_OUT, 0777) < 0)
			return;
	}
#ifdef DEBUG
	fcmd = stdin;
#else
	fcmd = fopen(P_FIFO, "r");
#endif

	//if the reader disconnects,then break
	while (fgets(cmdbuff, CMD_BUFF_SIZE, fcmd) > 0) {
#ifdef DEBUG
		fout = stdout;
#else
		fout = fopen(FIFO_OUT, "w");
#endif
		params = sscanf(cmdbuff, "%s %s %s %s %s", param[0], param[1], param[2],
				param[3], param[4]);
		if (strcmp(param[0], CMD_GETLIST) == 0) {
			getlist();
		} else if (strcmp(param[0], CMD_SET_SELECT) == 0) {
			if (params != 2) {
				_ERROR_();
			} else {
				setselect(param[1]);
			}
		} else if (strcmp(param[0], CMD_SET_PMODE) == 0) {
			if (params != 2) {
				_ERROR_();
			} else {
				setpmode(param[1]);
			}
		} else if (strcmp(param[0], CMD_OPENCAP) == 0) {
			_SUCCESS_();
			opencap();
		} else if (strcmp(param[0], CMD_CLOSECAP) == 0) {
			_SUCCESS_();
			closecap();
		} else if (strcmp(param[0], CMD_SETFILTER) == 0) {
			if (params != 2) {
				_ERROR_();
			} else {
				//use '_' replaces ' '
				int i;
				for (i = 0; param[1][i]; i++)
					if (param[1][i] == '_')
						param[1][i] = ' ';
				setfilter(param[1]);
			}
		}
#ifndef DEBUG
		fclose(fout);
#endif
	}
	fclose(fcmd);
}

void doserve() {
	while (1) {
		serveone();
	}
}

int main() {
	doserve();
	return 0;
}
