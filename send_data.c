#include <stdio.h>
#include <iwlib.h>
#include <wireless.h>


int main(int argc, char *argv[]){

	iw_enum_handler fn;
	int x = iw_sockets_open();
	printf("\nThe socket is %d\n",x);
	int version=iw_get_kernel_we_version();
	printf("The version of WE is %d\n",version);
	iw_enum_devices(3, fn,argv,3);
	return 0;
}