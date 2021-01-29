#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

// Here FILE  is a structure that holds the information about the file
// Here FILE *fpipe is a file pointer 

	FILE *fpipe;
	char *command = "ls -l";
	char c =0;
	int my_size=0;
// popen() function shall execute the command specified by the string command.
// It shall create a pipe between the calling program and the executed command
// and shall return a pointer to the stream that can be either used to read or write
// to the pipe

	if ( 0 == (fpipe =(FILE*)popen(command,"r")))
	{
		perror("popen() failed");
		exit(EXIT_FAILURE);
	}
	printf("%d\n",sizeof(c));

// size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
// ptr - this is pointer to a block of memory with a minimum size of size *nmemb bytes
// size - This is the size in bytes of each element to be read
// nmemb - This is the number of elements, each one with a size of size bytes
// stream - THis is the pointer to the FILE object that specifies an input stream

	while (fread(&c,sizeof c, 1, fpipe))
	{
		printf("%c",c);
	}
	pclose(fpipe);
	return EXIT_SUCCESS;
}
