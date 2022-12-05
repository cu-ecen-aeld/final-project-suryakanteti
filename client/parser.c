#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

void tokenize(char* packet, gps_data_t* gps, accelerometer_data_t* accl)
{
    int ret;
    ret = sscanf(packet,
                 "%lf %c %lf %c %lf %d %d %d",
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
    printf("Latitude: %lf\n", gpsData.lat);
    printf("Latitude Direction: %s\n", gpsData.lat_direction);
    printf("Longitude: %lf\n", gpsData.lon);
    printf("Longitude Direction: %s\n", gpsData.lat_direction);
    printf("UTC Time: %lf\n\n", gpsData.utc);

    // Accelerometer data
    printf("Acceleration in X-axis: %d\n", acclData.x_value);
    printf("Acceleration in X-axis: %d\n", acclData.y_value);
    printf("Acceleration in X-axis: %d\n\n", acclData.z_value);
}
