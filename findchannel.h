#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Channel
{
	int frequency;
	int busy_time;
	int active_time;
	int transmit_time;
	int channel;
};

void print_channel(struct Channel);
void parse_info(struct Channel *, FILE **);
int getCurrentChannel(struct Channel *);

