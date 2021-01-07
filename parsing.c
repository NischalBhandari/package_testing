#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Channel
{
	int frequency;
	int busy_time;
	int active_time;
	int transmit_time;
};

void parse_channels(struct Channel *, FILE **);
void print_channels(struct Channel *, FILE **);

int main(void)
{
	FILE *fp;
	char *myfile="test.txt";
	char c;
//	char line[40];
	int i=0;
	struct Channel Ch[13];
//	char param1[10],param2[10],param3[10],timing[10],active_time[10];
	if( 0 == (fp=(FILE*)fopen(myfile,"r")))
	{
		perror("fopen() failed");
		exit(EXIT_FAILURE);
	}
	parse_channels(Ch, &fp);


/*	while(fscanf(fp,"%79[^\n]\n",line)==1)
	{
		if(strstr(line,"active")){
			sscanf(line, "%s%s%s%s",param1,param2,param3,active_time);
			Ch[i].active_time=atoi(active_time);
			printf("actual active time: %s\n",active_time);
		}
		if(strstr(line,"busy")){
			sscanf(line,"%s%s%s%s",param1,param2,param3,timing);
			Ch[i].busy_time=atoi(timing);
			printf("actual busy_time : %d\n",atoi(timing));
			i+=1;
	}
	}
*/
	for(int i=0;i<13;i++)
	{
		printf("active time: %d && busy time : %d\n",Ch[i].active_time,Ch[i].busy_time);
	}

//	while(fread(&c,sizeof c, 1 ,fp))
//	{
//		printf("%c",c);
//	}


	fclose(fp);
	return EXIT_SUCCESS;
}

void parse_channels(struct Channel *Ch, FILE **fp){
		int i = 0;
        char param1[10],param2[10],param3[10],timing[10],active_time[10];
	char line[40];
	        while(fscanf(*fp,"%79[^\n]\n",line)==1)
        {
                if(strstr(line,"active")){
                        sscanf(line, "%s%s%s%s",param1,param2,param3,active_time);
                        Ch[i].active_time=atoi(active_time);
                        printf("actual active time: %s\n",active_time);
                }
                if(strstr(line,"busy")){
                        sscanf(line,"%s%s%s%s",param1,param2,param3,timing);
                        Ch[i].busy_time=atoi(timing);
                        printf("actual busy_time : %d\n",atoi(timing));
                        i+=1;
        }
        }


}
