typedef struct{
double lat;
char lat_direction;
double lon;
char lon_direction;
double UTC_time;
}gps_data;

typedef struct{
 float axis_value_1;
 char axis_1;
 float aixis_value_2;
 char axis_2;
 float axis_value_3;
 char axis_3;
}accelerometer_data;

void parse(char* packet);