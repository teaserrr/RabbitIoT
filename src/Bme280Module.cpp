#include "Bme280Module.h"

Bme280Module::Bme280Module(const char* id, unsigned long readInterval, const char* baseMqttPath)
    : BaseModule(id, readInterval, baseMqttPath, NULL, NULL) {
        _bme = NULL;
}

Bme280Module::~Bme280Module() {
    if (_bme != NULL) {
        delete _bme;
        // delete strings that were created with concat()
        delete _temperature->getMqttTopic();
        delete _humidity->getMqttTopic();
        delete _pressure->getMqttTopic();
        delete _temperature;
        delete _humidity;
        delete _pressure;
    }
}

void Bme280Module::setup() {
    _log.debug("Setting up Bme280Module...");
    _bme = new Adafruit_BME280();
    _temperature = new Measurement(_log, "temperature", "Temperature", "°C", concat(getMqttPath(), "temperature"));
    _humidity = new Measurement(_log, "humidity", "Humidity", "%", concat(getMqttPath(), "humidity"));
    _pressure = new Measurement(_log, "pressure", "Pressure", "hPa", concat(getMqttPath(), "pressure"), 0, I_1HOUR);
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

    FloatData temperature = FloatData(_bme->readTemperature());
    FloatData humidity = FloatData(_bme->readHumidity());
    FloatData pressure = FloatData(_bme->readPressure() / 100.0F);

    _log.log(LOGLEVEL_DEBUG, "temperature: %f", temperature.value());
    _log.log(LOGLEVEL_DEBUG, "humidity: %f", humidity.value());
    _log.log(LOGLEVEL_DEBUG, "pressure: %f", pressure.value());

    _temperature->updateValue(temperature);
    _humidity->updateValue(humidity);
    _pressure->updateValue(pressure);
}
