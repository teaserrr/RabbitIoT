#include "Bme280Module.h"

Bme280Module::Bme280Module(String id, unsigned long readInterval)
    : BaseModule(id, readInterval, NULL, NULL) {
    _bme = new Adafruit_BME280();
    _temperature = new FloatMeasurement("temperature");
    _humidity = new FloatMeasurement("humidity");
    _pressure = new FloatMeasurement("pressure");
    addMeasurement(_temperature);
    addMeasurement(_humidity);
    addMeasurement(_pressure);
}

Bme280Module::~Bme280Module() {
    delete _bme;
}

void Bme280Module::setup() {
    _log.debug("Initializing BME280 sensor...");
    if (!_bme->begin(0x76)) {   
        _log.error("Could not find a valid BME280 sensor");
        setEnabled(false);
    }
    else {        
        _log.info("BME280 ok");
    }    
}

void Bme280Module::loopInner() {
    _bme->takeForcedMeasurement();
    _temperature->updateValue(_bme->readTemperature());
    _humidity->updateValue(_bme->readHumidity());
    _pressure->updateValue(_bme->readPressure() / 100.0F);

    _log.debug("temperature: " + _temperature->getStringValue());
    _log.debug("humidity: " + _humidity->getStringValue());
    _log.debug("pressure: " + _pressure->getStringValue());
}
