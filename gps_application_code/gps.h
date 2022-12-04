#ifndef __GPS_H__
#define __GPS_H__

#include <stdbool.h>

#define MESSAGE_MAX_LENGTH 128

struct gpgll_s {

	char latitude[15];
	char latitudeDir;
	char longitude[15];
	char longitudeDir;
	char utc[15];
	bool dataStatus;

};

enum GpsMessageType
{
	GPGGA,
	GPGSA,
	GPGSV,
	GPGLL,
	GPRMC,
	GPVTG
};

int OpenPort();

int ReadMessage(int dataFd);

void HandleMessage(char* gpsMessage, int messageSize);

#endif // !__GPS_H__
