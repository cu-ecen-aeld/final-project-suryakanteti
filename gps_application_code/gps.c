#include "gps.h"
#include "misc.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define ssize_t int

//#define GPS_FILE "/dev/hw_serial-48022000"
#define GPS_FILE "gps_output.txt"

int startPos = 0;

void GpgllHandler(char* gpsMessage, int messageSize);
void PrintGpgllMesg(struct gpgll_s* msgPtr);

int OpenPort()
{
	int dataFd = open(GPS_FILE, O_RDONLY);
	if (dataFd == -1)
	{
		// Error
		perror("OpenPort");
		return -1;
	}
	return dataFd;
}

int ReadMessage(int dataFd)
{
	char gpsMessage[MESSAGE_MAX_LENGTH];
	ssize_t bytesRead = 0, totalBytesRead = 0;

	char* msgPtr = gpsMessage;

	// Read one message until \n encountered
	while (totalBytesRead <= MESSAGE_MAX_LENGTH)
	{
		bytesRead = read(dataFd, msgPtr, sizeof(char));
		if (bytesRead == -1)
		{
			perror("ReadMessage read()");
			return -1;
		}

		totalBytesRead += bytesRead;

		if (*msgPtr == '\n')
		{
			*msgPtr = '\0';
			break;
		}

		msgPtr++;
	}

	printf("GPS Message: %s\n", gpsMessage);
	HandleMessage(gpsMessage, totalBytesRead);
	return totalBytesRead;
}

void HandleMessage(char* gpsMessage, int messageSize)
{
	char messageType[6];

	// Parse message type
	messageType[5] = '\0';
	strncpy(messageType, gpsMessage + 1, 5);

	if (strcmp(messageType, "GPGLL") == 0)
	{
		GpgllHandler(gpsMessage, messageSize);
	}
	else
	{
		printf("Invalid command!\n");
	}
}

void GpgllHandler(char* gpsMessage, int messageSize)
{
	// 8 fields including the message type
	int pos1, pos2;
	struct gpgll_s msgObj;

	// Field 2
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 1); // End of field 1
	pos2 = FindCharacter(gpsMessage, messageSize, '.', 1);

	strncpy(msgObj.latitude, gpsMessage + pos1 + 1, pos2 - pos1 + 2); // Upto 2 decimal points
	msgObj.latitude[pos2 - pos1 + 2] = '\0';

	// Field 3
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 2); // End of field 2 
	msgObj.latitudeDir = *(gpsMessage + pos1 + 1);

	// Field 4
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 3);
	pos2 = FindCharacter(gpsMessage, messageSize, '.', 2);

	strncpy(msgObj.longitude, gpsMessage + pos1 + 1, pos2 - pos1 + 2); // 2 decimal ponits
	msgObj.longitude[pos2 - pos1 + 2] = '\0';

	// Field 5
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 4); // End of field 4 
	msgObj.longitudeDir = *(gpsMessage + pos1 + 1);

	// Field 6
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 5); // End of field 5
	pos2 = FindCharacter(gpsMessage, messageSize, ',', 6);

	strncpy(msgObj.utc, gpsMessage + pos1 + 1, pos2 - pos1 - 1);
	msgObj.utc[pos2 - pos1 - 1] = '\0';

	PrintGpgllMesg(&msgObj);
}

void PrintGpgllMesg(struct gpgll_s* msgPtr)
{
	printf("Latitude: %s\n", msgPtr->latitude);
	printf("Latitude Direction: %c\n", msgPtr->latitudeDir);

	printf("Longitude: %s\n", msgPtr->longitude);
	printf("Longitude Direction: %c\n", msgPtr->longitudeDir);

	printf("UTC: %s\n", msgPtr->utc);
}