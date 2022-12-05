#include "gps.h"
#include <stdio.h>


int main()
{
	char temp[MESSAGE_MAX_LENGTH];

	int dataFd = OpenPort();
	if (dataFd == -1)
	{
		printf("OpenPort() failed!\n");
		return -1;
	}

	while (1)
	{
		PopulateGpsData(dataFd, temp, MESSAGE_MAX_LENGTH);
	}

	return 0;
}