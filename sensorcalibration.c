#include <stdbool.h>
#include "thermalmod.h"
#include "consts.h"

extern thrd_t calibration_th, reading_th;
extern mtx_t mutex_calib;
extern mtx_t mutex_read;

void* periodic_sensors_callibaration(void * ptr)
{
    while (true) {
        mtx_lock(&mutex_calib);
        mtx_lock(&mutex_read);
        callibrate_sensors(ALL_SENSORS);
        sleep(500 * 1000);
        mtx_unlock(&mutex_read);
        mtx_unlock(&mutex_calib);
    }
    return NULL;
}

void callibrate_sensors(SENSORS sensors) {
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
