#ifndef Bme280Module_h
#define Bme280Module_h

#include <BaseModule.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/**
 * Implementation of a module for a BME280 environment sensor.
 * This module has 3 measurements:
 * - temperature: float value in °C
 * - humidity: float value in %
 * - pressure: float value in hPa
 * 
 * The values for these measurements will be published as a topic with the provided base path prepended to the measurement name.
 * E.g. with baseMqttPath set to "sensor/environment/" the published topics' paths will be: 
 *   sensor/environment/temperature
 *   sensor/environment/humidity
 *   sensor/environment/pressure
 *
 * The readInterval determines how often the sensor data is read, updated and published.
 * Note that the pressure data is only published once an hour.
 */
class Bme280Module : public BaseModule {
    public:
        Bme280Module(const char* id = "BME280", unsigned long readInterval = 10000, const char* baseMqttPath = "bme280/");
        ~Bme280Module();

        virtual void setup();

    protected:
        virtual void loopInner();
        
        Adafruit_BME280* _bme;

    private:
        Measurement* _temperature;
        Measurement* _humidity;
        Measurement* _pressure;
};

#endif
