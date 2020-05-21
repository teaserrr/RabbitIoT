#include "Bme280Module.h"

#define ID_TEMPERATURE "temperature"
#define ID_HUMIDITY "humidity"
#define ID_PRESSURE "pressure"


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
    _log.debug(PSTR("Setting up Bme280Module..."));
    _bme = new Adafruit_BME280();
    _temperature = new Measurement(_log, ID_TEMPERATURE, "Temperature", "°C", concat(getMqttPath(), ID_TEMPERATURE));
    _humidity = new Measurement(_log, ID_HUMIDITY, "Humidity", "%", concat(getMqttPath(), ID_HUMIDITY));
    _pressure = new Measurement(_log, ID_PRESSURE, "Pressure", "hPa", concat(getMqttPath(), ID_PRESSURE), 0, I_1HOUR);
    addMeasurement(_temperature);
    addMeasurement(_humidity);
    addMeasurement(_pressure);

    _log.debug(PSTR("Initializing BME280 sensor..."));
    if (!_bme->begin(0x76)) {   
        _log.error(PSTR("Could not find a valid BME280 sensor"));
        setEnabled(false);
    }
    else {     
        _bme->setSampling(Adafruit_BME280::MODE_FORCED, 
                          Adafruit_BME280::SAMPLING_X1, // temperature 
                          Adafruit_BME280::SAMPLING_X1, // pressure 
                          Adafruit_BME280::SAMPLING_X1, // humidity 
                          Adafruit_BME280::FILTER_OFF);    
        _log.info(PSTR("BME280 ok"));
    }    
}

void Bme280Module::loopInner() {
    _bme->takeForcedMeasurement();

    FloatData temperature = FloatData(_bme->readTemperature());
    FloatData humidity = FloatData(_bme->readHumidity());
    FloatData pressure = FloatData(_bme->readPressure() / 100.0F);

    _log.log(LOGLEVEL_DEBUG, PSTR("temperature: %f"), temperature.value());
    _log.log(LOGLEVEL_DEBUG, PSTR("humidity: %f"), humidity.value());
    _log.log(LOGLEVEL_DEBUG, PSTR("pressure: %f"), pressure.value());

    _temperature->updateValue(temperature);
    _humidity->updateValue(humidity);
    _pressure->updateValue(pressure);
}
