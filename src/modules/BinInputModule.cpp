#include "BinInputModule.h"

BinInputModule::BinInputModule(const char* id, uint8_t pin, unsigned long loopDelay, const char* mqttPath)
    : BaseModule(id, loopDelay, mqttPath, NULL, NULL) {
        _pin = pin;
        _state = NULL;
}

BinInputModule::~BinInputModule() {
    if (_state != NULL) {
        delete _state;
    }
}

void BinInputModule::setup() {
    _state = new Measurement(_log, "state", "Input state", "", getMqttPath());
    _log.debug(PSTR("Setting up Binary input module..."));
    addMeasurement(_state);
    pinMode(_pin, INPUT);
}

void BinInputModule::loopInner() {
    bool newState = digitalRead(_pin);
    _log.log(LOGLEVEL_TRACE, PSTR("%s state: %s"), getId(), newState ? PSTR("high") : PSTR("low"));

    _state->updateValue(BoolData(newState));
}

bool BinInputModule::getState() {
    if (_state->data())
        return ((BoolData*)_state->data())->value();
    return false;
} 