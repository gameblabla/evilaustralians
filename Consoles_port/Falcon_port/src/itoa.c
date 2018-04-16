#include <tos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 *http://www.strudel.org.uk/itoa/
*/

char* itoa_own(unsigned short val)
{
	static char buf[5] = {0};
	unsigned char i = 5;
	for(; val && i ; --i, val /= 10)
		buf[i] = "0123456789"[val % 10];
	return &buf[i+1];
}
