
#ifndef _input_h_
#define _input_h_

#include <threads.h>

typedef enum SENSORS {ALL_SENSORS, UPPER_SENSOR, LOWER_SENSOR, LEFT_SENSOR, RIGHT_SENSOR} SENSORS;

/* gets a digit from the input device */
int get_input_digit();

/* prints a string to the output device */
int print_string(char*);

/* prints a string array to the output device */
int print_string_array();

void* periodic_sensors_callibaration(void * ptr);

void * periodic_sensors_scan(void * ptr);

void callibrate_sensors(SENSORS);

void read_sensors(SENSORS);

extern thrd_t calibration_th, reading_th;

extern mtx_t mutex_calib;

extern mtx_t mutex_read;

#endif /* _input_h_ */

