#include "BinInputModule.h"

BinInputModule::BinInputModule(const char* id, uint8_t pin, unsigned long loopDelay, const char* mqttPath)
    : BaseModule(id, loopDelay, NULL, NULL) {
        _pin = pin;
        _mqttPath = mqttPath;
}

BinInputModule::~BinInputModule() {
    if (_state != NULL) {
        delete _state;
    }
}

void BinInputModule::setup() {
    _log.debug("Setting up Binary input module...");
    _state = new Measurement(_log, "state", "Input state", "", _mqttPath);
    addMeasurement(_state);
    pinMode(_pin, INPUT);
}

void BinInputModule::loopInner() {
    bool newState = digitalRead(_pin);
    _log.log(LOGLEVEL_DEBUG, "state: %s", newState ? "high" : "low");

    _state->updateValue(BoolData(newState));
}
