#include <pcap.h>
#include <string.h>
#include <arpa/inet.h>

#include "pcapfunc.h"

/*
 * this is the cgi main 
 * router file,It's work
 * is deal the get request(like www.baidu.com?param1=blabla)
 * and call functions declared in pcapfunc.h 
 * 
 */

int main() {
	char *action;
	char *name;
	char *expr;

	if (cgl_initenv() == -1) {
		cgl_perror(stderr, "testcgl");
		exit(1);
	}
	if (cgl_initformdata() == -1) {
		cgl_perror(stderr, "testcgl");
		exit(1);
	}
	cgl_content_header("text/plain");
	action = cgl_getvalue("action");
	name = cgl_getvalue("name");
	expr = cgl_getvalue("expr");
	if (action == NULL) {
		printf("No Action defined!");
		exit(1);
	}
	if (strcmp("inter_list", action) == 0) {
		getlist();
	} else if (strcmp("switch_select", action) == 0) {
		if (name != NULL)
			switch_select(name);
		else
			printf("No name defined!");
	} else if (strcmp("switch_pmode", action) == 0) {
		if (name != NULL)
			switch_pmode(name);
		else
			printf("No name defined");
	} else if (strcmp("getcap", action) == 0) {
		getcap();
	} else if (strcmp("opencap", action) == 0) {
		opencap();
	} else if (strcmp("closecap", action) == 0) {
		closecap();
	} else if (strcmp("setfilter", action) == 0) {
		if (expr != NULL)
			setfilter(expr);
		else
			printf("No expr defined!");
	} else if (strcmp("startserver", action) == 0) {
		startserver();
	}
	return 0;
}
