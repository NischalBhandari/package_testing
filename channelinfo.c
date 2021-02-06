#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "findchannel.h"
#include "channelinfo.h"
// struct Channel
// {
// 	int frequency;
// 	int busy_time;
// 	int active_time;
// 	int transmit_time;
// };

int get_all_channels(struct Channel *Ch)
{
	FILE *fp;
//	char *mycommand="iw wlan0 survey dump";

	struct Channel best_channel;
	//read the file that needs to be checked for frequency other information
	if( 0 == (fp=(FILE*)fopen("survey.txt","r")))
	{
		perror("fopen() failed");
		exit(EXIT_FAILURE);
	}

	//function to parse the channels
	parse_channels(Ch, &fp);

	//function to print the channels
	print_channels(Ch);

	find_best_channel(Ch, &best_channel);
	print_best_channel(best_channel);
//	change_channel(best_channel);
	//close the file pointer
	fclose(fp);

	return EXIT_SUCCESS;
}

void parse_channels(struct Channel *Ch, FILE **fp){
//this function parses the channel to the Channel Structure using the FILE pointer passed
//to it
	int i = 0;
        char param1[10],param2[10],param3[10],timing[10],active_time[10];
	char frequency[10];
	char line[40];
	while(fscanf(*fp,"%79[^\n]\n",line)==1)
        	{	if(strstr(line,"frequency")){
				sscanf(line,"%s%s",param1,frequency);
				Ch[i].frequency=atoi(frequency);
//				printf("actual frequency: %s\n",frequency);
			}
                	if(strstr(line,"active")){
                        	sscanf(line, "%s%s%s%s",param1,param2,param3,active_time);
                        	Ch[i].active_time=atoi(active_time);
//                        	printf("actual active time: %s\n",active_time);
                	}
                	if(strstr(line,"busy")){
                        	sscanf(line,"%s%s%s%s",param1,param2,param3,timing);
                        	Ch[i].busy_time=atoi(timing);
//                        	printf("actual busy_time : %d\n",atoi(timing));
                        	i+=1;
        		}
        }


}

void print_channels(struct Channel *Ch){
// this function prints the channels and their information to the stdout
	int i=0;
	for(i=0;i<13;i++){
 		printf(" frequency: %d active time: %d && busy time : %d\n",Ch[i].frequency,Ch[i].active_time,Ch[i].busy_time);
	}
}



void find_best_channel(struct Channel *Ch, struct Channel *best_channel){
	// this function finds the best channel from the list of scanned channels
	int lowest_busy_time=Ch[0].busy_time;
	int channel_flag=0;
	int i=0;
	for(i=0;i<13;i++)
	{
		if(lowest_busy_time > Ch[i].busy_time)
		{
			lowest_busy_time=Ch[i].busy_time;
			channel_flag=i;
		}
	}
			//assigns the designated channel to the best_channel struct
	*best_channel = Ch[channel_flag];
}

void print_best_channel(struct Channel Ch){
	printf("%d is the best channel with %d busy time and %d active time \n ", Ch.frequency,Ch.busy_time,Ch.active_time);
}

/*void change_channel(struct Channel Ch){
	system(uci set wireless.radio0.channel='
}*/
void change_channel(struct Channel Ch){
	char buf[40];
        int err1, err2, err3;
	int channel = ((Ch.frequency-2400)-7)/5;
	snprintf(buf,sizeof(buf),"uci set wireless.radio0.channel=%d",channel);
	printf("%s\n",buf);
	err1=system(buf);
	err2=system("uci commit");
	err3=system("reload_config");
        printf("Changed to channel %d\n error1 %d error2 %d error3 %d",channel,err1,err2,err3);
}


