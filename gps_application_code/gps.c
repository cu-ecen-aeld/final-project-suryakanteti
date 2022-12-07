/**
* Author: Surya Kanteti
*/

#include "gps.h"
#include "misc.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// GPS port based on the current driver
#define GPS_FILE "/dev/hw_serial-48022000"

// Message structure to store GPGLL data
struct gpgll_s msgObj;

// Functions used within the code but not provided as interfaces
int ReadMessage(int dataFd);
void HandleMessage(char* gpsMessage, int messageSize);
void GpgllHandler(char* gpsMessage, int messageSize);

/**
 * @brief: Opens the port to read GPS data
 *
 * @return File descriptor of the opened port, -1 in case of error
 * 
*/
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

/**
 * @brief: Populates GPS data into a fixed size buffer
 *
 * @param dataFd: File descriptor for GPS data port
 * @param[out] buffer: Buffer to be filled
 * @param length: Length of the buffer
 *
 * @return Integer indicating the position of the delimiter, -1 if not found
 * 
*/
int PopulateGpsData(int dataFd, char* buffer, int size)
{
	if (buffer == NULL || size <= 0)
		return -1;

	// Read one line and handle it
	int messageSize = ReadMessage(dataFd);
	if (messageSize == -1)
	{
		printf("ReadMessage() failed!\n");
		return -1;
	}

	// Populate a dummy string
	char temp[MESSAGE_MAX_LENGTH];
	char* tmpPtr = temp;

	if (msgObj.dataStatus == true)
	{

		// Latitude
		strncpy(tmpPtr, msgObj.latitude, strlen(msgObj.latitude));
		tmpPtr += strlen(msgObj.latitude);
		*tmpPtr++ = ' ';

		// Latitude direction
		*tmpPtr++ = msgObj.latitudeDir;
		*tmpPtr++ = ' ';

		// Longitude
		strncpy(tmpPtr, msgObj.longitude, strlen(msgObj.longitude));
		tmpPtr += strlen(msgObj.longitude);
		*tmpPtr++ = ' ';

		// Longitude direction
		*tmpPtr++ = msgObj.longitudeDir;
		*tmpPtr++ = ' ';

		// UTC
		strncpy(tmpPtr, msgObj.utc, strlen(msgObj.utc));
		tmpPtr += strlen(msgObj.utc);

		*tmpPtr = '\0';

		if (strlen(temp) > size)
		{
			printf("Not enough space in buffer to populate GPS data!\n");
			return -1;
		}
		else
		{
			strncpy(buffer, temp, strlen(temp));
		}
	}
	else
	{
		strncpy(buffer, "0 0 0 0 0", 9 * sizeof(char));
		return 9;
	}
	
	return strlen(temp);
}

/**
 * @brief: Prints GPGLL message on the console
 *
 * @param msgPtr: Pointer to the GPGLL message structure
 * 
 * @return None
 * 
*/
void PrintGpgllMesg(struct gpgll_s* msgPtr)
{
	if (msgPtr->dataStatus != false)
	{
		printf("\nLatitude: %s\n", msgPtr->latitude);
		printf("Latitude Direction: %c\n", msgPtr->latitudeDir);

		printf("Longitude: %s\n", msgPtr->longitude);
		printf("Longitude Direction: %c\n", msgPtr->longitudeDir);

		printf("UTC: %s\n\n", msgPtr->utc);
	}
	else
	{
		printf("Invalid data!\n");
	}
}

/**
 * @brief: Reads one message from the GPS data
 *
 * @param dataFd: File descriptor for GPS data port
 *
 * @return Total bytes read, -1 in case of error
 * 
*/
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

	HandleMessage(gpsMessage, totalBytesRead);
	return totalBytesRead;
}

/**
 * @brief: Parses the message and calls the respective handler
 *
 * @param gpsMessage: String containing the GPS message
 * @param messageSize: Size of the GPS message
 *
 * @return None
 * 
*/
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
}

/**
 * @brief: Handler for GPGLL messages. Populates the global GPGLL structure
 *
 * @param gpsMessage: String containing the GPS message
 * @param messageSize: Size of the GPS message
 *
 * @return None
 * 
*/
void GpgllHandler(char* gpsMessage, int messageSize)
{
	// 8 fields including the message type
	int pos1, pos2;
	msgObj.dataStatus = true;

	// Field 2: Latitude
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 1); // End of field 1
	pos2 = FindCharacter(gpsMessage, messageSize, ',', 2);

	if (pos2 == pos1 + 1)
	{
		*msgObj.latitude = '\0';
		msgObj.dataStatus = false;
	}
	else
	{
		strncpy(msgObj.latitude, gpsMessage + pos1 + 1, pos2 - pos1 - 1);
	}

	msgObj.latitude[pos2 - pos1 - 1] = '\0';

	// Field 3: :Latitude Direction
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 2); // End of field 2
	pos2 = FindCharacter(gpsMessage, messageSize, ',', 3);

	if (pos2 == pos1 + 1)
	{
		msgObj.latitudeDir = '\0';
		msgObj.dataStatus = false;
	}
	else
	{
		msgObj.latitudeDir = *(gpsMessage + pos1 + 1);
	}

	// Field 4: Longitude
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 3); // End of field 3
	pos2 = FindCharacter(gpsMessage, messageSize, ',', 4);

	if (pos2 == pos1 + 1)
	{
		*msgObj.longitude = '\0';
		msgObj.dataStatus = false;
	}
	else
	{
		strncpy(msgObj.longitude, gpsMessage + pos1 + 1, pos2 - pos1 - 1);
	}

	msgObj.longitude[pos2 - pos1 - 1] = '\0';

	// Field 5: Longitude Direction
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 4); // End of field 4
	pos2 = FindCharacter(gpsMessage, messageSize, ',', 5);

	if (pos2 == pos1 + 1)
	{
		msgObj.longitudeDir = '\0';
		msgObj.dataStatus = false;
	}
	else
	{
		msgObj.longitudeDir = *(gpsMessage + pos1 + 1);
	}

	// Field 6: UTC
	pos1 = FindCharacter(gpsMessage, messageSize, ',', 5); // End of field 5
	pos2 = FindCharacter(gpsMessage, messageSize, ',', 6);

	if (pos2 == pos1 + 1)
	{
		*msgObj.utc = '\0';
		msgObj.dataStatus = false;
	}
	else
	{
		strncpy(msgObj.utc, gpsMessage + pos1 + 1, pos2 - pos1 - 1);
	}

	msgObj.utc[pos2 - pos1 - 1] = '\0';
}