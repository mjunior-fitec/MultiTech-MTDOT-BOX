/**
 * @file    SensorHandler.cpp
 * @brief   Reads on board sensors... acceleration, pressure, light and temperture.
 * @author  Leon Lindenfelser
 * @version 1.0
 *
 */

#include "SensorHandler.h"

SensorHandler::SensorHandler()
  : _getSensorThread(&SensorHandler::startSensorThread, this),
    _mDoti2c(PC_9,PA_8),
	_accelerometer(_mDoti2c,MMA845x::SA0_VSS),
	_barometricSensor(_mDoti2c),
	_lightSensor(_mDoti2c)
{
    SensorHandler::initSensors();
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
        _mutex.lock();        
        _accelerometerData = _accelerometer.getXYZ();
        _mutex.unlock();
	 
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
        _mutex.lock();        
        _pressure = _barometricSensor.getBaroData() >> 12; // convert 32 bit signed to 20 bit unsigned value
        _mutex.unlock();
        
        // Trigger an Altitude reading
        _barometricSensor.setParameters(MPL3115A2::DATA_NORMAL, MPL3115A2::DM_ALTIMETER, MPL3115A2::OR_16,
        					MPL3115A2::AT_1);
        _barometricSensor.triggerOneShot();
	 
        // Test barometer device status to see if acquisition is complete
        do {
            osDelay(100);			 // allows other threads to process
            result = _barometricSensor.getStatus();
        } while ((result & MPL3115A2::PTDR) == 0 );
	 
        // Retrieve temperature and altitude.
        _mutex.lock();
        _barometerData = _barometricSensor.getAllData(false);
        _mutex.unlock();        
	 
        // Retrieve light level
        _mutex.lock();
        _light = _lightSensor.getData();
        _mutex.unlock();        
    }
}

MMA845x_DATA SensorHandler::getAcceleration(){
    MMA845x_DATA data;
    _mutex.lock();
    data = _accelerometerData;
    _mutex.unlock();
    return data;
}

float SensorHandler::getLight(){
    float light;
    uint16_t whole;
    _mutex.lock();
    whole = _light;
    _mutex.unlock();
    light = whole * 24 % 100;
    light /= 100;
    light = light + (whole * 24 / 100);       // 16000 lux full scale .24 lux per bit
    return light; 
}

uint16_t SensorHandler::getLightRaw(){
    uint16_t light;
    _mutex.lock();
    light = _light;
    _mutex.unlock();
    return light; 
}

float SensorHandler::getPressure(){
    float pressure;
    uint32_t whole;
    _mutex.lock();
    whole = _pressure;
    _mutex.unlock();
    pressure = (whole & 3) * .25;
    pressure = pressure + (whole >> 2);
    return pressure;    
}

uint32_t SensorHandler::getPressureRaw(){
    uint32_t pressure;
    _mutex.lock();
    pressure = _pressure;
    _mutex.unlock();
    return pressure;    
}

float SensorHandler::getTemp(Scale scale){
    float temperature;
    uint16_t whole;
    _mutex.lock();
    whole = _barometerData._temp;    
    _mutex.unlock();
    temperature = whole & 0x0f;
    temperature *= .0625;
    temperature += (whole >> 4);
    if(scale == FAHRENHEIT){
        temperature = temperature * 1.8 + 32;
    }
    return temperature;
}

float SensorHandler::getAltitude(){
    float altitude;
    uint32_t whole;
    _mutex.lock();
    whole = _barometerData._baro;    
    _mutex.unlock();
    whole /= 4096;
    altitude = (whole & 0x0f) * .0625;
    whole /= 16;
    altitude += whole;
    return altitude;
}

MPL3115A2_DATA SensorHandler::getBarometer(){
    MPL3115A2_DATA data;
    _mutex.lock();
    data = _barometerData;
    _mutex.unlock();
    return data;
}

