#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "findchannel.h"

int getCurrentChannel(struct Channel *my_channel)
{
	FILE *fp;
//	char *mycommand="iw wlan0 info | grep channel ";

	//read the file that needs to be checked for frequency other information
	if( 0 == (fp=(FILE*)fopen("output.txt","r")))
	{
		perror("fopen() failed");
		exit(EXIT_FAILURE);
	}

	//function to parse the channels
	parse_info(my_channel,&fp);

	print_channel(*my_channel);
	//function to print the channels
	//print_channels(Ch);

	//find_best_channel(Ch, &best_channel);
	//print_best_channel(best_channel);
//	change_channel(best_channel);
	//close the file pointer
	fclose(fp);

	return EXIT_SUCCESS;
}

void parse_info(struct Channel *Ch, FILE **fp){
	char line[80];
	char garbage1[20];
	char garbage2[20];
	char garbage3[30];
	int channel;
	char freq[20];
	int freq_int;
	int width;
	while(fscanf(*fp, "%79[^\n]\n",line)==1)
	{
		sscanf(line,"%s%d%s%s%d%s",garbage1,&channel,freq,garbage2,&width,garbage3);
		printf("%d\n",channel);
		freq[0] = '0';
		freq_int=atoi(freq);
		(*Ch).frequency=freq_int;
		(*Ch).channel =channel;

	}
}
void print_channel(struct Channel Ch){
	printf(" frequency: %d and channel is %d \n",Ch.frequency,Ch.channel);

}

