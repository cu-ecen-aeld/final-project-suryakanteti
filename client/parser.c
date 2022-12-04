#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

void tokenize(char* packet,gps_data* gps_data_parsed){
int ret;
ret=sscanf(packet,"%lf %c %lf %c %lf",&gps_data_parsed->lat,&gps_data_parsed->lat_direction,&gps_data_parsed->lon,&gps_data_parsed->lon_direction,&gps_data_parsed->UTC_time);
}

void parse(char* packet){
gps_data gps_data_t;
tokenize(packet,&gps_data_t);
printf("Latitude :%lf",gps_data_t.lat);
printf("Latitude Direction:%c",gps_data_t.lat_direction);
printf("Longitude :%lf",gps_data_t.lon);
printf("Longitude Direction:%c",gps_data_t.lat_direction);
printf("UTC TIme :%lf",gps_data_t.UTC_time);
}
