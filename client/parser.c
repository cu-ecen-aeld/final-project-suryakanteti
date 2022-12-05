#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

// "X: %d, Y: %d, Z: %d"

void tokenize(char* packet, gps_data_t* gps, accelerometer_data_t* accl)
{
    int ret;
    ret = sscanf(packet,
                 "%lf,%c,%lf,%c,%lf,X: %d, Y: %d, Z: %d\n",
                 &gps->lat, &gps->lat_direction, &gps->lon, &gps->lon_direction, &gps->utc,
                 &accl->x_value, &accl->y_value, &accl->z_value);
}

void parse(char* packet)
{
    gps_data_t gpsData;
    accelerometer_data_t acclData;

    // Parse
    tokenize(packet, &gpsData, &acclData);

    // GPS data
    printf("Latitude: %lf", gpsData.lat);
    printf("Latitude Direction: %c", gpsData.lat_direction);
    printf("Longitude: %lf", gpsData.lon);
    printf("Longitude Direction: %c", gpsData.lat_direction);
    printf("UTC Time: %lf", gpsData.UTC_time);

    // Accelerometer data
    printf("Acceleration in X-axis: %d", acclData.x_value);
    printf("Acceleration in X-axis: %d", acclData.y_value);
    printf("Acceleration in X-axis: %d", acclData.z_value);
}
