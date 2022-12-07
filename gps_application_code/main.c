/**
* Author: Surya Kanteti
*/

#include "gps.h"
#include <stdio.h>


int main()
{
	char temp[MESSAGE_MAX_LENGTH];

	// Open the port
	int dataFd = OpenPort();
	if (dataFd == -1)
	{
		printf("OpenPort() failed!\n");
		return -1;
	}

	while (1)
	{
		// Populate data into string
		int gpsBytes = PopulateGpsData(dataFd, temp, MESSAGE_MAX_LENGTH);
		temp[gpsBytes] = '\0';
		printf("GPS String: %s\n", temp);
	}

	return 0;
}