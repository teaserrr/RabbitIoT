#ifndef BinOutputModule_h
#define BinOutputModule_h

#include "BaseModule.h"

/**
 * Implementation of a module for writing to a digital output pin.
 * This module has 1 measurement:
 * - state: boolean value (unitless)
 * 
 * The value of this measurement will be published as a topic with the specified path. 
 * A HIGH pin state will be published as "1", LOW as "0".
 * 
 * The loopDelay determines how often the input pin is read and published.
 * Whenever the state of the pin is different from the last time is was read, its state is published.
 */
class BinOutputModule : public BaseModule {
    public:
        BinOutputModule(const char* id, uint8_t pin, const char* mqttPath = NULL);
        ~BinOutputModule();

        virtual void setup();

        void setState(bool state);
        bool getState();

    protected:
        virtual void loopInner();
        
    private:
        bool _state;
        uint8_t _pin;
};

#endif
