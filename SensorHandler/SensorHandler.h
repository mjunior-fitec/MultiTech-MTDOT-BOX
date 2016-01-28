/* Copyright (c) <2016> <MultiTech Systems>, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file    SensorHandler.h
 * @brief   Reads on board sensors... acceleration, pressure, light and temperture.
 * @author  Leon Lindenfelser
 * @version 1.0
 *
 */

#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "mbed.h"
#include "MMA845x.h"
#include "MPL3115A2.h"
#include "ISL29011.h"
#include "DOGS102.h"
#include "rtos.h"
#include <string>
#include <vector>
#define START_THREAD 1

class SensorHandler
{
public:
    enum Scale{CELSIUS, FAHRENHEIT};
    SensorHandler(void);
    ~SensorHandler(void);
    MMA845x_DATA getAcceleration(void);
    float getLight(void);
    uint16_t getLightRaw(void);
    float getPressure(void);
    uint32_t getPressureRaw(void);
    float getTemp(Scale);
    float getAltitude(void);
    MPL3115A2_DATA getBarometer(void);
		
private:
    Thread _getSensorThread;
    static void startSensorThread (void const *p);	
    void readSensors(void);
    void initSensors(void);	
    I2C _mDoti2c;
    MMA845x _accelerometer;
    MPL3115A2 _barometricSensor;
    ISL29011 _lightSensor;
    MMA845x_DATA _accelerometerData;
    MPL3115A2_DATA _barometerData;
    uint16_t _light;
    uint32_t _pressure;
    Mutex _mutex;	
};
	
#endif //SENSORHANDLER_H
