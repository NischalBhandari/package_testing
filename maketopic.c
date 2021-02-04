#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int appendtopics(char *mac_addr, char *argv, char *topic){
	printf("The topics side by side are %s and %s where mac address is %d\n",mac_addr,argv,strlen(mac_addr));
	memset(topic,0,30);
	strcpy(topic,argv);
	int len=strlen(argv);
	int i=1;
	topic[len]='/';
	printf("The length of topic is %d",strlen(topic));
	while(mac_addr[i]!='\0'){
		
		if(mac_addr[i] == ':'){
			printf("found a :\n");
			topic[len+i]='q';
		}
		else{
		topic[len+i]=mac_addr[i];
		printf("%c\n",mac_addr[i]);
		}
		i=i+1;
	}
		printf("The length of topic is %d\n",strlen(topic));
	return 0;
}