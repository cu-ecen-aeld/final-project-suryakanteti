/**
* Author: Surya Kanteti
*/

#ifndef __GPS_H__
#define __GPS_H__

#include <stdbool.h>

// Max length of one GPS message
#define MESSAGE_MAX_LENGTH 128

// Structure to store the GPGLL message
struct gpgll_s {

	char latitude[15];
	char latitudeDir;
	char longitude[15];
	char longitudeDir;
	char utc[15];
	bool dataStatus;

};

// Different GPS message types
enum GpsMessageType
{
	GPGGA,
	GPGSA,
	GPGSV,
	GPGLL,
	GPRMC,
	GPVTG
};

/**
 * @brief: Opens the port to read GPS data
 *
 * @return File descriptor of the opened port, -1 in case of error
 * 
*/
int OpenPort();

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
int PopulateGpsData(int dataFd, char* buffer, int length);

/**
 * @brief: Prints GPGLL message on the console
 *
 * @param msgPtr: Pointer to the GPGLL message structure
 * 
 * @return None
 * 
*/
void PrintGpgllMesg(struct gpgll_s* msgPtr);

#endif // !__GPS_H__
