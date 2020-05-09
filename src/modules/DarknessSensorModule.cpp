#include "DarknessSensorModule.h"

DarknessSensorModule::DarknessSensorModule(const char* id, uint8_t pin, unsigned long loopDelay, unsigned short threshold, unsigned short hysteresis, const char* mqttPath) 
    : BaseModule(id, loopDelay, mqttPath, NULL, NULL) {
    _threshold = threshold;
    _hysteresis = hysteresis;
    _state = NULL;
}

DarknessSensorModule::~DarknessSensorModule() {
    if (_state != NULL) {
        delete _state;
    }
}

void DarknessSensorModule::setup() {
    _log.debug("Setting up Darkness Sensor module...");
    _state = new Measurement(_log, "state", "Darkness state", "", getMqttPath());
    _log.debug("Add measurement...");
    addMeasurement(_state);
    _log.debug("set pinmode...");
    pinMode(_pin, INPUT);
    _log.debug("Setup done.");
}

void DarknessSensorModule::loopInner() {
    unsigned long lightValue = analogRead(_pin);
    bool newState;

    if (isDarkness()) {
        newState = lightValue <= _threshold + _hysteresis;
    }
    else {
        newState = lightValue <= _threshold;
    }
    
    _log.log(LOGLEVEL_DEBUG, "light value: %u - state: %s", lightValue, newState ? "darkness" : "light");

    _state->updateValue(BoolData(newState));
}

bool DarknessSensorModule::isDarkness() {
    return _state && _state->data() && ((BoolData*)_state->data())->value();
}