#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

/**
 * @brief: Parses the packet into provided structures
 *
 * @param packet: Received packet in string format
 * @param[out] gps: Structure pointer to the GPS data
 * @param[out] accl: Structure pointer to the accelerometer data
 *
 * @return None
 * 
*/
void tokenize(char* packet, gps_data_t* gps, accelerometer_data_t* accl)
{
    int ret;
    ret = sscanf(packet,
                 "%lf %c %lf %c %lf %d %d %d ",
                 &gps->lat, &gps->lat_direction, &gps->lon, &gps->lon_direction, &gps->utc,
                 &accl->x_value, &accl->y_value, &accl->z_value);
}

/**
 * @brief: Parses the the sensor data and prints on the console
 *
 * @param packet: Received packet in string format
 *
 * @return File descriptor of the opened port, -1 in case of error
 * 
*/
void parse(char* packet)
{
    gps_data_t gpsData;
    accelerometer_data_t acclData;

    // Parse
    tokenize(packet, &gpsData, &acclData);

    // GPS data
    printf("Latitude: %lf\n", gpsData.lat);
    printf("Latitude Direction: %c\n", gpsData.lat_direction);
    printf("Longitude: %lf\n", gpsData.lon);
    printf("Longitude Direction: %c\n", gpsData.lon_direction);
    printf("UTC Time: %lf\n\n", gpsData.utc);

    // Accelerometer data
    printf("Acceleration in X-axis: %d\n", acclData.x_value);
    printf("Acceleration in Y-axis: %d\n", acclData.y_value);
    printf("Acceleration in Z-axis: %d\n\n", acclData.z_value);
}
