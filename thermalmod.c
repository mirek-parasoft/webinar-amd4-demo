/* 
 * Implements routines from inout.h for stdin/stdout
 */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdatomic.h>
#include "thermalmod.h"
#include "consts.h"

/* calibration and sensors scanning threads */
thrd_t calibration_th, reading_th;

/* callibration and scanning require exclusive access to sensors */
mtx_t mutex_calib;
mtx_t mutex_read;

/* Thermal state structure */
typedef struct temp_sensors {
 uint16_t upper_sensor;
 uint16_t lower_sensor;
 uint16_t left_sensor;
 uint16_t right_sensor;
} temp_sensors;

/* Current thermal state */
_Atomic temp_sensors thermal_state;

/* Sensors HW access */
static temp_sensors get_thermal_state_hw();

/* */
void update_thermal_state()
{
    temp_sensors current_state = get_thermal_state_hw();
    atomic_store(&thermal_state, current_state);
}

/* read data from senors every 10 seconds*/
void * periodic_sensors_scan(void * ptr)
{
    while (true) {
        mtx_lock(&mutex_read);
        mtx_lock(&mutex_calib);
        read_sensors(ALL_SENSORS);
        sleep(10);
        mtx_unlock(&mutex_calib);
        mtx_unlock(&mutex_read);
    }
    return NULL;
}

void initialize()
{
    int rc_cal, rc_read;

    rc_read = thrd_create(&reading_th, (thrd_start_t) periodic_sensors_scan, (void *) NULL);
    rc_cal  = thrd_create(&calibration_th, (thrd_start_t) periodic_sensors_callibaration, (void *)NULL);
    
    if (rc_cal == thrd_error || rc_read == thrd_error) {
        printf("ERORR; thrd_create() call failed\n");
        exit(EXIT_FAILURE);
    }
}

temp_sensors get_thermal_state_hw() {
    temp_sensors sensors;
    return sensors;
}

void read_sensors(SENSORS sensors) {
    switch (sensors)
    {
        case ALL_SENSORS:
            break;
        case UPPER_SENSOR:
            break;
        case LOWER_SENSOR:
            break;
        case LEFT_SENSOR:
            break;
        case RIGHT_SENSOR:
            break;
        default:
            break;
    }
}

/*
 * Grabs input from stdin
 */
int get_input_digit()
{
    char c;
    int i;
    char buf[BUF_SIZE];

#ifdef STDINPUT
    /* define STDINPUT to be able to accept input from a console */
    for (i = 0; i < BUF_SIZE; i++) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            break;
        } else if (isdigit(c)) {
            buf[i] = c;
        }
    }
#else /* STDINPUT */

    /* to allow automated unit testing */
    static int index = 0;

    /* simulate user input */
    ++ index;
    index %= 10;
    buf[0] = (char)('0' + index);
    i = 1;

#endif /* STDINPUT */
    
    buf[i] = '\0';
    return (atoi(buf));
}

/*
 * Print string to stdout
 */
int print_string(char* str)
{
    return (printf(str));
}
