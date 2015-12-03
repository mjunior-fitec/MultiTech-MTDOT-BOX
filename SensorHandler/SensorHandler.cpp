/**
 * @file    SensorHandler.cpp
 * @brief   Reads on board sensors... acceleration, pressure, light and temperture.
 * @author  Leon Lindenfelser
 * @version 1.0
 *
 */

#include "SensorHandler.h"

SensorHandler::SensorHandler()
  : _getSensorThread(&SensorHandler::startSensorThread,this),
    _mDoti2c(PC_9,PA_8),
	_accelerometer(_mDoti2c,MMA845x::SA0_VSS),
	_barometricSensor(_mDoti2c),
	_lightSensor(_mDoti2c)
{
    _getSensorThread.signal_set(START_THREAD);
    return;
}

SensorHandler::~SensorHandler(void)
{
    _getSensorThread.terminate();
}     

void SensorHandler::initSensors(){
    //  Setup the Accelerometer for 8g range, 14 bit resolution, Noise reduction off, sample rate 1.56 Hz
    //  normal oversample mode, High pass filter off
    _accelerometer.setCommonParameters(MMA845x::RANGE_8g,MMA845x::RES_MAX,MMA845x::LN_OFF,
    						   MMA845x::DR_1_56,MMA845x::OS_NORMAL,MMA845x::HPF_OFF );
	 
    // Setup the Barometric sensor for post processed Ambient pressure, 4 samples per data acquisition.
    //and a sample taken every second when in active mode
    _barometricSensor.setParameters(MPL3115A2::DATA_NORMAL, MPL3115A2::DM_BAROMETER, MPL3115A2::OR_16,
    					MPL3115A2::AT_1);
    // Setup the Ambient Light Sensor for continuous Ambient Light Sensing, 16 bit resolution,
    // and 16000 lux range
    _lightSensor.setMode(ISL29011::ALS_CONT);
    _lightSensor.setResolution(ISL29011::ADC_16BIT);
    _lightSensor.setRange(ISL29011::RNG_16000);
	 
    // Set the accelerometer for active mode
    _accelerometer.activeMode();
	 
    // Clear the min-max registers in the Barometric Sensor
    _barometricSensor.clearMinMaxRegs();    
}

void SensorHandler::startSensorThread(void const *p)
{
    SensorHandler *instance = (SensorHandler*)p;
    instance->readSensors();
}

void SensorHandler::readSensors()
{
    uint8_t result;
    _getSensorThread.signal_wait(START_THREAD);
    while(1){
        // Test Accelerometer XYZ data ready bit to see if acquisition complete
        do {
            osDelay(100);			 // allows other threads to process
            result = _accelerometer.getStatus();
        } while ((result & MMA845x::XYZDR) == 0 );
	 
        // Retrieve accelerometer data
        _accelerometerData = _accelerometer.getXYZ();
	 
        // Trigger a Pressure reading
        _barometricSensor.setParameters(MPL3115A2::DATA_NORMAL, MPL3115A2::DM_BAROMETER, MPL3115A2::OR_16,
									MPL3115A2::AT_1);
        _barometricSensor.triggerOneShot();
	 
        // Test barometer device status to see if acquisition is complete
        do {
            osDelay(100);			 // allows other threads to process
            result = _barometricSensor.getStatus();
        } while ((result & MPL3115A2::PTDR) == 0 );
	 
        // Retrieve barometric pressure
        _pressure = _barometricSensor.getBaroData() >> 12; // convert 32 bit signed to 20 bit unsigned value
	 
        // Trigger a Altitude reading
        _barometricSensor.setParameters(MPL3115A2::DATA_NORMAL, MPL3115A2::DM_ALTIMETER, MPL3115A2::OR_16,
        					MPL3115A2::AT_1);
        _barometricSensor.triggerOneShot();
	 
        // Test barometer device status to see if acquisition is complete
        do {
            osDelay(100);			 // allows other threads to process
            result = _barometricSensor.getStatus();
        } while ((result & MPL3115A2::PTDR) == 0 );
	 
        // Retrieve temperature
        _barometerData = _barometricSensor.getAllData(false);
	 
        // Retrieve light level
        _light = _lightSensor.getData();
    }
}

MMA845x_DATA SensorHandler::getAcceleration(){
    return _accelerometerData;
}

uint16_t SensorHandler::getLight(){
    return _light;
}

uint32_t SensorHandler::getPressure(){
    return _pressure;
}

MPL3115A2_DATA SensorHandler::getBarometer(){
    return _barometerData;
}

