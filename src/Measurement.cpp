#include "Measurement.h"
#include <Arduino.h>

Measurement::Measurement(const Logger& logger, const char* id, const char* description, const char* unit, const char* mqttTopic, unsigned long minPublishInterval , unsigned long maxPublishInterval) {
    _log = logger;
    _id = id;
    _description = description;
    _unit = unit;
    _mqttTopic = mqttTopic;
    _minPublishInterval = minPublishInterval;
    _maxPublishInterval = maxPublishInterval;
    _isUpdated = false;
    _lastUpdate = 0;
    _lastPublish = 0;
    _data = NULL;
    _firstTimePublish = true;

    for (int i=0; i<MAX_STATELISTENERS; i++) {
        _stateListeners[i] = NULL;
    }
}

Measurement::~Measurement() {
}

void Measurement::updateValue(const BaseData& value) {
    _log.log(LOGLEVEL_TRACE, PSTR("%s updateValue: %s - currentValue: %s"), _id, value.stringValue(), _data ? _data->stringValue() : "NULL");
    if (!_data || !_data->equals(value)) { 
        if (_data) delete _data;
        _data = value.copy();
        updateTimestamp();
        setUpdated();
        _log.log(LOGLEVEL_INFO, PSTR("%s updateValue - updated: %s"), _id, _data->stringValue());
        notifyListeners();
    }
}

bool Measurement::shouldPublish() const {
    return _mqttTopic && 
            data() &&
            (
                (_isUpdated && (intervalExceeded(_maxPublishInterval) || _maxPublishInterval == 0 || _firstTimePublish))
                || intervalExceeded(_minPublishInterval)
            );
}

void Measurement::setPublished() {
    if (!_isUpdated)
        updateTimestamp();
    _lastPublish = millis();
    resetUpdated();
}

void Measurement::resetUpdated() { 
    _isUpdated = false; 
    _firstTimePublish = false; 
}

void Measurement::addStateListener(StateListener* listener) {
    int i = 0;
    while (_stateListeners[i] && i < MAX_STATELISTENERS) {        
        i++;
    }
    if (i < MAX_STATELISTENERS)
        _stateListeners[i] = listener;
    else
        _log.log(LOGLEVEL_WARNING, PSTR("Measurement %s: Maximum amount of listeners exceeded. Increase MAX_STATELISTENERS to fix this."), _id);
}

void Measurement::notifyListeners() {
    int i = 0;
    while (_stateListeners[i] && i < MAX_STATELISTENERS) {
        _stateListeners[i]->onStateChanged(this);
        i++;
    }
}

void Measurement::updateTimestamp(unsigned long timestamp) {
    _lastUpdate = timestamp == 0 ? millis() : timestamp;
}
bool Measurement::intervalExceeded(unsigned long interval) const {
    return interval > 0 && millis() - _lastPublish > interval; 
}
