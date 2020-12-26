#ifndef PresenceDetectorModule_h
#define PresenceDetectorModule_h

#include "BinInputModule.h"

/**
 * A module for detecting presence using a movement detector on a digital input pin.
 * 
 * The movement detector can be a PIR or RF detector. Its signal is used in combination with
 * a timeout setting to determince presence. When the input signal becomes high, this module
 * will report 'presence'. When the signal becomes low, a delay is applied before 'no presence'
 * is reported.
 * 
 * This module has 1 measurement:
 * - state: boolean value (unitless)
 * 
 * This module has 1 configuration parameter:
 * - delay: integer value (seconds), default 30
 * 
 * The state of the presence detection will be published as a topic with the specified path. 
 * Presence will be published as "1", no presence as "0".
 */
class PresenceDetectorModule : public BinInputModule {
    public:
        PresenceDetectorModule(const char* id, uint8_t pin, const char* mqttPath = NULL);
        ~PresenceDetectorModule();

        virtual void setup();

    protected:
        virtual void loopInner();
        
    private:
        bool _sensorState;
        unsigned long _lastOffTimestamp;
        ConfigParameter* _cDelay;
};

#endif
