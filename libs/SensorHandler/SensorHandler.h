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
#include "rtos.h"
#include "MTSSerial.h"
#include <string>
#include <vector>
#include <ctime>
#define START_THREAD 1

class SensorHandler
{
public:
	MMA845x_DATA getAcceleration();
	uint16_t getLight();
	uint_32t getPressure();
	MPL3115A2_DATA getTemp();


private:
    Thread _getSensorThread;
	void readSensors(void);
	I2C _mDoti2c;
	MMA845x _accelerometer;
	MPL3115A2 _barometricSensor;
	ISL29011 _lightSensor;
}
	
#endif //SENSORHANDLER_H
