#include "gps.h"
#include <stdio.h>


int main()
{
	// Open the GPS port
	int dataFd = OpenPort();
	if (dataFd == -1)
	{
		printf("OpenPort() failed!\n");
		return -1;
	}

	while (1)
	{
		// Read one line
		int messageSize = ReadMessage(dataFd);
		if (messageSize == -1)
		{
			printf("ReadMessage() failed!\n");
			return -1;
		}
	}


	// Parse to form a structure and return it...maybe only important things
	// Display the structure
	// On the client side, to get it, we might have to serialize data and de-serialize it

	return 0;
}