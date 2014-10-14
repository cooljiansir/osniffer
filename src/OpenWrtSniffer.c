#include <pcap.h>
#include <string.h>
#include <arpa/inet.h>


void getlist();

int main(){
	char *action;
	
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
	if(action==NULL){
		printf("No Action defined!");
		exit(1);
	}
	if(strcmp("inter_list",action)==0){
		getlist();
	}else {
		
	}
	return 0;
}
