
/* 
 * Implements routines from inout.h for stdin/stdout
 */

#include "thermalmod.h"
#include "consts.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <threads.h>

thrd_t calibration_th, reading_th;


mtx_t lock_calibration;
mtx_t lock_reading;

typedef struct temp_sensors {
 uint16_t upper_sensor;
 uint16_t lower_sensor;
 uint16_t left_sensor;
 uint16_t right_sensor;
} temp_sensors;

_Atomic temp_sensors thermal_state;

temp_sensors get_thermal_state_hw();

void update_thermal_stat1e()
{
    temp_sensors current_state = get_thermal_state_hw();
    thermal_state.upper_sensor = current_state.upper_sensor;
    thermal_state.lower_sensor = current_state.lower_sensor;
    thermal_state.left_sensor = current_state.left_sensor;
    thermal_state.right_sensor = current_state.right_sensor;
}

void update_thermal_state()
{
    temp_sensors current_state = get_thermal_state_hw();
    thermal_state.upper_sensor = current_state.upper_sensor;
    thermal_state.lower_sensor = current_state.lower_sensor;
    thermal_state.left_sensor = current_state.left_sensor;
    thermal_state.right_sensor = current_state.right_sensor;
}

void* periodic_sensors_callibaration(void * ptr)
{
    while (true) {
        mtx_lock(&lock_calibration);
        mtx_lock(&lock_reading);
        sleep(500 * 1000);
        mtx_unlock(&lock_reading);
        mtx_unlock(&lock_calibration);
    }
    return NULL;
}

void * periodic_sensors_read(void * ptr)
{
    while (true) {
        temp_sensors current_value;
        mtx_lock(&lock_reading);
        mtx_lock(&lock_calibration);
        sleep(50);
        mtx_unlock(&lock_calibration);
        mtx_unlock(&lock_reading);
    }
    return NULL;
}

void initialize()
{
    int rc_cal, rc_read;

    rc_cal  = thrd_create(&calibration_th, (thrd_start_t) periodic_sensors_callibaration, (void *)NULL);
    rc_read = thrd_create(&reading_th, (thrd_start_t) periodic_sensors_read, (void *) NULL);

    if (rc_cal == thrd_error || rc_read == thrd_error) {
        printf("ERORR; thrd_create() call failed\n");
        exit(EXIT_FAILURE);
    }
}

temp_sensors get_thermal_state_hw() {
    temp_sensors sensors;
    return sensors;
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
