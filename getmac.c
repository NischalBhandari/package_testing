#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int get_mac( char *mac_addr){
	char c;
	int point=0;
	FILE *fp = fopen("/sys/class/net/enp0s3/address","r");
	if ( fp == NULL){
		printf("Cannot open file\n");
		exit(0);
	}
	//The C library function int fgetc(FILE *stream) gets the 
	//next character (an unsigned char) from the specified stream and advances the position indicator for the stream.
	c = fgetc(fp);
	while (c != EOF){
		mac_addr[point]=c;
		c=fgetc(fp);
		point = point + 1;
	}
	fclose(fp);
	return 0;


}
