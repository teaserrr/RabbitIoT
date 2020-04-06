#ifndef Bme280Module_h
#define Bme280Module_h

#include <BaseModule.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class Bme280Module : public BaseModule {
    public:
        Bme280Module(String id = "BME280", unsigned long readInterval = 10000);
        ~Bme280Module();

        virtual void setup();

    protected:
        virtual void loopInner();
        
        Adafruit_BME280* _bme;

};

#endif
