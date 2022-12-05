typedef struct {

    char lat[15];
    char lat_direction;
    char lon[15];
    char lon_direction;
    char utc[15];

} gps_data_t;

typedef struct {

    int x_value;
    int y_value;
    int z_value;

} accelerometer_data_t;

void parse(char* packet);