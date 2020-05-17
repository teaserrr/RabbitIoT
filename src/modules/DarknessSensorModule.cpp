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
        delete _mState;
        // delete strings that were created with concat()
        delete _mState->getMqttTopic();
        delete _cThreshold->getMqttTopic();
        delete _cHysteresis->getMqttTopic();
    }
}

void DarknessSensorModule::setup() {
    _log.debug("Setting up Darkness Sensor module...");
    pinMode(_pin, INPUT);

    _mState = new Measurement(_log, ID_STATE, "Darkness state", "", concat(getMqttPath(), ID_STATE));
    addMeasurement(_mState);
    
    _cThreshold = new ConfigParameter(ID_THRESHOLD, "Darkness threshold", _dThreshold, concat(getMqttPath(), ID_THRESHOLD));
    _cHysteresis = new ConfigParameter(ID_HYSTERESIS, "Darkness hysteresis", _dHysteresis, concat(getMqttPath(), ID_HYSTERESIS));
    addConfigParameter(_cThreshold);
    addConfigParameter(_cHysteresis);

    _log.debug("DarknessSensorModule setup done.");
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
    
    _log.log(LOGLEVEL_DEBUG, "light value: %u - state: %s", lightValue, newState ? "darkness" : "light");

    _mState->updateValue(BoolData(newState));
}

bool DarknessSensorModule::isDarkness() {
    return _mState && _mState->data() && ((BoolData*)_mState->data())->value();
}