#include "Arduino.h"
#include <Wire.h>

#ifndef GY_85_h
#define GY_85_h

//----------addresses----------//
#define ADXL345 (0x53)         // Device address as specified in data sheet
#define DATAX0  (0x32)         // X-Axis Data 0
#define HMC5883 (0x1E)         // gyro
#define ITG3200 (0x68)         // compass


class GY_85 {

private:
    void GyroCalibrate();

    void SetGyro();

    void SetCompass();

    void SetAccelerometer();

public:
    void begin();

    int *readFromAccelerometer();

    int *readFromCompass();

    float *readGyro();

    //callback functions
    inline int accelerometer_x(int *a) { return *(a); }

    inline int accelerometer_y(int *a) { return *(1 + a); }

    inline int accelerometer_z(int *a) { return *(2 + a); }

    //-----------------------------------

    inline int compass_x(int *a) { return *(a); }

    inline int compass_y(int *a) { return *(1 + a); }

    inline int compass_z(int *a) { return *(2 + a); }

    //-----------------------------------

    inline float gyro_x(float *a) { return *(a) / 14.375; }

    inline float gyro_y(float *a) { return *(1 + a) / 14.375; }

    inline float gyro_z(float *a) { return *(2 + a) / 14.375; }

    inline float temp(float *a) { return 35 + (*(3 + a) + 13200) / 280; }
};

#endif
