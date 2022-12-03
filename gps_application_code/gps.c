#include "gps.h"
#include "misc.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

unsigned long startPoint = 0;

#define GPS_FILE "/dev/hw_serial-48022000"
//#define GPS_FILE "gps_output.txt"

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

int GetToValidMessage(int dataFd)
{
	ssize_t rc;
	char c = '\0';
	int pos = 0;

	while (1)
	{
		while (c != '$')
		{
			rc = read(dataFd, &c, sizeof(char));
			pos++;
			if (rc == -1)
			{
				perror("GetToValidMessage read()");
				return -1;
			}
		}

		rc = read(dataFd, &c, sizeof(char));
		if (rc == -1)
		{
			perror("GetToValidMessage read() 1");
			return -1;
		}

		if (c == 'G')
			break;
	}

	if (lseek(dataFd, pos, SEEK_SET) == -1)
	{
		perror("GetToValidMessage lseek");
		return -1;
	}

	startPoint += pos;

	return 0;
}

int ReadMessage(int dataFd, char* gpsMessage)
{
	ssize_t rc;

	// Read until the max length
	rc = read(dataFd, gpsMessage, MESSAGE_MAX_LENGTH * sizeof(char));
	if (rc == -1)
	{
		perror("ReadMessage read()");
		return -1;
	}

	// Find the first \n
	int pos = FindCharacter(gpsMessage, MESSAGE_MAX_LENGTH, '\n', 1);
	if (pos == -1)
	{
		// Logically shouldn't happen given that max message length is checked for
		printf("Couldn't find instance number 1 of delimiter in the messages\n");
		return -1;
	}

	// Adjust where to read from
	startPoint += pos + 2;
	if (lseek(dataFd, startPoint, SEEK_SET) == -1)
	{
		perror("ReadMessage lseek");
		return -1;
	}

	gpsMessage[pos] = '\0';
	return pos + 1;
}

void HandleMessage(char* gpsMessage, int messageSize)
{
	char messageType[6];

	// Parse message type
	messageType[5] = '\0';
	strncpy(messageType, gpsMessage, 5);

	printf("Message Type: %s\n", messageType);

	if (strcmp(messageType, "GPGLL") == 0)
	{
		GpgllHandler(gpsMessage, messageSize);
	}
	/*else
	{
		printf("Invalid command!\n");
	}*/
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
