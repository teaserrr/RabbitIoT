#include "PresenceDetectorModule.h"

#define PARAM_DELAY "presenceDelay"

PresenceDetectorModule::PresenceDetectorModule(const char* id, uint8_t pin, const char* mqttPath)
    : BinInputModule(id, pin, 1000, mqttPath) {
}

PresenceDetectorModule::~PresenceDetectorModule() {
    if (_cDelay != NULL)
        // delete strings that were created with concat()
        delete _cDelay->getMqttTopic();
}

void PresenceDetectorModule::setup() {
    BinInputModule::setup();

    _cDelay = new ConfigParameter(PARAM_DELAY, PSTR("Presence detector delay"), 30, concat(getMqttPath(), "delay"));
    addConfigParameter(_cDelay);
}

void PresenceDetectorModule::loopInner() {
    bool newState = digitalRead(_pin);

    if (newState != _sensorState) {
        _log.log(LOGLEVEL_DEBUG, PSTR("%s new state: %s"), getId(), newState ? PSTR("high") : PSTR("low"));

        if (newState) {
            // rising edge; publish immediately
            _state->updateValue(BoolData(newState));
        }
        else if (!newState) {
            // falling edge, reset timestamp
            _lastOffTimestamp = millis();
        }
    }
    _sensorState = newState;

    if (!_sensorState && (millis() - _lastOffTimestamp > _cDelay->getIntValue() * 1000)) {
        // input still low after delay
        _state->updateValue(BoolData(_sensorState));
    }
}
