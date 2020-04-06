#include "Bme280Module.h"

Bme280Module::Bme280Module(String id, unsigned long readInterval)
    : BaseModule(id, readInterval, NULL, NULL) {
    _bme = new Adafruit_BME280();
}

Bme280Module::~Bme280Module() {
    delete _bme;
}

void Bme280Module::setup() {
    Serial.println("Initializing BME280 sensor...");
    if (!_bme->begin(0x76)) {   
        Serial.println("Could not find a valid BME280 sensor");
        setEnabled(false);
    }
    else {        
        Serial.println("BME280 ok");
    }    
}

void Bme280Module::loopInner() {
    _bme->takeForcedMeasurement();
    float temperature = _bme->readTemperature();
    float humidity = _bme->readHumidity();
    float pressure = _bme->readPressure() / 100.0F;

    Serial.print("temperature: ");
    Serial.println(temperature);
    Serial.print("humidity: ");
    Serial.println(humidity);
    Serial.print("pressure: ");
    Serial.println(pressure);
}
