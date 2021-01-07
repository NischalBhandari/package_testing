#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(void)
{
	FILE *fp;
	char *myfile="test.txt";
	char c;
	char line[40];
	int i=0;
	int busy_time[13]={0};
	char param1[10],param2[10],param3[10],timing[10];
	if( 0 == (fp=(FILE*)fopen(myfile,"r")))
	{
		perror("fopen() failed");
		exit(EXIT_FAILURE);
	}
	i=0;
	while(fscanf(fp,"%79[^\n]\n",line)==1)
	{
		if(strstr(line,"busy")){
			sscanf(line, "%s%s%s%s",param1,param2,param3,timing);
			busy_time[i]=atoi(timing);
			printf("This is i : %d\n",i);
			printf("%d\n",atoi(timing));
			i=i+1;
		}
	}

	for(int i=0;i<13;i++)
	{
		printf("%d\n",busy_time[i]);
	}

//	while(fread(&c,sizeof c, 1 ,fp))
//	{
//		printf("%c",c);
//	}


	fclose(fp);
	return EXIT_SUCCESS;
}

