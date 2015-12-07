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
	SensorHandler(void);
	~SensorHandler(void);
	MMA845x_DATA getAcceleration(void);
	uint16_t getLight(void);
	uint32_t getPressure(void);
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
