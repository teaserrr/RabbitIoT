#ifndef DarknessSensorModule_h
#define DarknessSensorModule_h

#include "BaseModule.h"

/**
 * Implementation of a module for determining darkness.
 * A darkness sensor can be achieved by connecting a simple LDR between VCC (*) and an analog input pin, and a resistor (e.g. 10K) between 
 * GND and the same pin. This puts the input pin at 0V when it's dark, and at VCC when it's light. A threshold value will determine
 * when the switch between dark and light is made. 
 * 
 * This module has 1 measurement:
 * - state: boolean value (unitless)
 * 
 * The value of this measurement will be published as a topic with the specified path. 
 * Darkness will be published as "1", light as "0".
 * 
 * The loopDelay determines how often the light value is measured and darkness state is evaluated.
 */
class DarknessSensorModule : public BaseModule {
    public:
        /**
         * Parameters:
         * - id: module id
         * - pin: pin number of an analog input pin (default 17 = A0)
         * - loopDelay: determines how often the light value is measured and darkness state is evaluated
         * - threshold: determines when to switch between darkness and light. Sensor values smaller than or equal to the threshold are considered darkness.
         * - hysteresis: allows to add some 'delay' before switching back from darkness to light. When the state is darkness, this value is added to the
         *               threshold; the state will only switch back to light if the sensor value is greater than threshold + hysteresis
         * - mqttPath: path used to publish the state topic
         */
        DarknessSensorModule(const char* id, uint8_t pin = 17 /* A0 */, unsigned long loopDelay = 1000, unsigned short threshold = 400, unsigned short hysteresis = 0, 
                             const char* mqttPath = NULL);
        ~DarknessSensorModule();

        virtual void setup();
        
        bool isDarkness();

    protected:
        virtual void loopInner();
        
    private:
        Measurement* _state;
        uint8_t _pin;
        unsigned short _threshold;
        unsigned short _hysteresis;
};

#endif
