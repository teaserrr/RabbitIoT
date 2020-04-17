#include "Bme280Module.h"

Bme280Module::Bme280Module(const String& id, unsigned long readInterval, const String& baseMqttPath)
    : BaseModule(id, readInterval, NULL, NULL) {
        _bme = NULL;
        _baseMqttPath = baseMqttPath;
}

Bme280Module::~Bme280Module() {
    if (_bme != NULL)
        delete _bme;
}

void Bme280Module::setup() {
    _log.debug("Setting up Bme280Module...");
    _bme = new Adafruit_BME280();
    _temperature = new FloatMeasurement("temperature", "Temperature", "°C", _baseMqttPath + "temperature");
    _humidity = new FloatMeasurement("humidity", "Humidity", "%", _baseMqttPath + "humidity");
    _pressure = new FloatMeasurement("pressure", "Pressure", "hPa", _baseMqttPath + "pressure");
    addMeasurement(_temperature);
    addMeasurement(_humidity);
    addMeasurement(_pressure);

    _log.debug("Initializing BME280 sensor...");
    if (!_bme->begin(0x76)) {   
        _log.error("Could not find a valid BME280 sensor");
        setEnabled(false);
    }
    else {     
        _bme->setSampling(Adafruit_BME280::MODE_FORCED, 
                          Adafruit_BME280::SAMPLING_X1, // temperature 
                          Adafruit_BME280::SAMPLING_X1, // pressure 
                          Adafruit_BME280::SAMPLING_X1, // humidity 
                          Adafruit_BME280::FILTER_OFF);    
        _log.info("BME280 ok");
    }    
}

void Bme280Module::loopInner() {
    _bme->takeForcedMeasurement();
    _temperature->updateValue(_bme->readTemperature());
    _humidity->updateValue(_bme->readHumidity());
    _pressure->updateValue(_bme->readPressure() / 100.0F);

    _log.log(LOGLEVEL_DEBUG, "temperature: %s", _temperature->getStringValue().c_str());
    _log.log(LOGLEVEL_DEBUG, "humidity: %s", _humidity->getStringValue().c_str());
    _log.log(LOGLEVEL_DEBUG, "pressure: %s", _pressure->getStringValue().c_str());
}
