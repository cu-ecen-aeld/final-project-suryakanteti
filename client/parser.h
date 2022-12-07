// Structure type to store the GPS data
typedef struct {

    double lat;
    char lat_direction;
    double lon;
    char lon_direction;
    double utc;

} gps_data_t;

// Structure type to store accelerometer data
typedef struct {

    int x_value;
    int y_value;
    int z_value;

} accelerometer_data_t;

/**
 * @brief: Parses the the sensor data and prints on the console
 *
 * @param packet: Received packet in string format
 *
 * @return File descriptor of the opened port, -1 in case of error
 * 
*/
void parse(char* packet);