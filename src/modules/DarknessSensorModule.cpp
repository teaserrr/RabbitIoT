#include "DarknessSensorModule.h"

#define ID_STATE "state"
#define ID_THRESHOLD "threshold"
#define ID_HYSTERESIS "hysteresis"

DarknessSensorModule::DarknessSensorModule(const char* id, uint8_t pin, unsigned long loopDelay, unsigned short defaultThreshold, 
                                            unsigned short defaultHysteresis, const char* baseMqttPath) 
    : BaseModule(id, loopDelay, baseMqttPath, NULL, NULL) {
    _dThreshold = defaultThreshold;
    _dHysteresis = defaultHysteresis;
    _mState = NULL;
    _cThreshold = NULL;
    _cHysteresis = NULL;
}

DarknessSensorModule::~DarknessSensorModule() {
    if (_mState != NULL) {
        // delete strings that were created with concat()
        delete _mState->getId();
        delete _mState->getMqttTopic();
        delete _cThreshold->getMqttTopic();
        delete _cHysteresis->getMqttTopic();
        delete _mState;
        delete _cHysteresis;
        delete _cThreshold;
    }
}

void DarknessSensorModule::setup() {
    _log.debug(PSTR("Setting up Darkness Sensor module..."));
    pinMode(_pin, INPUT);

    _mState = new Measurement(_log, concat(getId(), ID_STATE), this, ID_STATE, "", concat(getMqttPath(), ID_STATE));
    addMeasurement(_mState);
    
    _cThreshold = new ConfigParameter(ID_THRESHOLD, PSTR("Darkness threshold"), _dThreshold, concat(getMqttPath(), ID_THRESHOLD));
    _cHysteresis = new ConfigParameter(ID_HYSTERESIS, PSTR("Darkness hysteresis"), _dHysteresis, concat(getMqttPath(), ID_HYSTERESIS));
    addConfigParameter(_cThreshold);
    addConfigParameter(_cHysteresis);

    _log.debug(PSTR("DarknessSensorModule setup done."));
}

void DarknessSensorModule::loopInner() {
    unsigned long lightValue = analogRead(_pin);
    bool newState;

    if (isDarkness()) {
        newState = lightValue <= _cThreshold->getIntValue() + _cHysteresis->getIntValue();
    }
    else {
        newState = lightValue <= _cThreshold->getIntValue();
    }
    
    _log.log(LOGLEVEL_DEBUG, PSTR("light value: %u - state: %s"), lightValue, newState ? PSTR("darkness") : PSTR("light"));

    _mState->updateValue(BoolData(newState));
}

bool DarknessSensorModule::isDarkness() {
    return _mState && _mState->data() && ((BoolData*)_mState->data())->value();
}