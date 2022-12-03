#include "gps.h"
#include "misc.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//#define ssize_t int

#define GPS_FILE "/dev/hw_serial-48022000"
//#define GPS_FILE "gps_output.txt"

char buffer[MESSAGE_MAX_LENGTH];
char gpsMessage[MESSAGE_MAX_LENGTH];

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

int ParseMessages(int dataFd)
{
	int pos = 0, endPos = 0;
	ssize_t bytesRead;

	// Read until the max length
	bytesRead = read(dataFd, buffer + startPos, (MESSAGE_MAX_LENGTH - startPos) * sizeof(char));
	if (bytesRead == -1)
	{
		perror("ReadMessage read()");
		return -1;
	}

	// Reach till the valid message
	while (1)
	{
		pos = FindCharacter(buffer, bytesRead, '$', 1);
		if (pos == -1)
		{
			printf("Couldn't find $ in the message");
			return -1;
		}

		if (*(buffer + pos + 1) == 'G')
		{
			break;
		}
	}

	// Adjust the buffer
	strncpy(buffer, buffer + pos, bytesRead - pos); // Should have $ as 1st character

	// Find the end of one message
	endPos = FindCharacter(buffer, startPos + bytesRead, '\n', 1);
	if (endPos == -1)
	{
		// Logically shouldn't happen given that max message length is checked for
		printf("Couldn't find instance number 1 of delimiter in the messages\n");
		return -1;
	}

	// Handle Gps message
	strncpy(gpsMessage, buffer, endPos); // Without \n
	gpsMessage[endPos] = '\0';
	HandleMessage(gpsMessage, endPos);
	
	// Adjust the buffer
	strncpy(buffer, buffer + endPos + 1, startPos + bytesRead - endPos - 1); // SURYA: Whatever is after endpos needs to be dragged to start
	startPos = startPos + bytesRead - endPos; // SURYA: Bytesread - Endpos becomes startPos
	return endPos;
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