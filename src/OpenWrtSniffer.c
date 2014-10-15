#include <pcap.h>
#include <string.h>
#include <arpa/inet.h>


void getlist();
void switch_select(char *name);
void switch_pmode(char *name);

int main(){
	char *action;
	char *name;
	
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
	if(action==NULL){
		printf("No Action defined!");
		exit(1);
	}
	if(strcmp("inter_list",action)==0){
		getlist();
	}else if(strcmp("switch_select",action)==0){
		if(name!=NULL)
			switch_select(name);
		else printf("No name defined!");
	}
	else if(strcmp("switch_pmode",action)==0){
		if(name!=NULL)
			switch_pmode(name);
		else printf("No name defined");
	}
	return 0;
}
