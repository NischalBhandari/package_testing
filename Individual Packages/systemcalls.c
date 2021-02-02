#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char* now()
{
	time_t t;
	time (&t);
	return asctime(localtime (&t));
}

int main()
{
	char comment[80];
	char cmd[120];
	//fgets for unstructured text
	fgets(comment,80,stdin);
	//Sprintf() will print the characters to a string format
	//The formatted string will be placed in cmd array
	sprintf(cmd,"echo '%s %s'>>reports.log",comment,now());
	system(cmd);
	return 0;

}