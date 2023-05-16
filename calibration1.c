#include <stdbool.h>
#include "thermalmod.h"
#include "consts.h"


extern thrd_t calibration_th, reading_th;
extern mtx_t mtx_calibration;
extern mtx_t mtx_reading;

void* periodic_sensors_callibaration1(void * ptr)
{
    while (true) {
        mtx_lock(&calib);
        mtx_lock(&read);
        callibrate_sensors(ALL_SENSORS);
        sleep(500 * 1000);
        mtx_unlock(&read);
        mtx_unlock(&calib);
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
