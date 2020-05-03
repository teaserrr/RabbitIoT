#ifndef BinInputModule_h
#define BinInputModule_h

#include <BaseModule.h>

/**
 * Implementation of a module for reading a digital input pin.
 * This module has 1 measurement:
 * - state: boolean value (unitless)
 * 
 * The value of this measurement will be published as a topic with the specified path. 
 * A HIGH pin state will be published as "1", LOW as "0".
 * 
 * The loopDelay determines how often the input pin is read and published.
 * Whenever the state of the pin is different from the last time is was read, its state is published.
 */
class BinInputModule : public BaseModule {
    public:
        BinInputModule(const char* id, uint8_t pin, unsigned long loopDelay = 1000, const char* mqttPath = NULL);
        ~BinInputModule();

        virtual void setup();

    protected:
        virtual void loopInner();
        
    private:
        Measurement* _state;
        uint8_t _pin;
};

#endif
