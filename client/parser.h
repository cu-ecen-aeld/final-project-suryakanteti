typedef struct {

    double lat;
    char lat_direction;
    double lon;
    char lon_direction;
    double utc;

} gps_data_t;

typedef struct {

    int x_value;
    int y_value;
    int z_value;

} accelerometer_data_t;

void parse(char* packet);