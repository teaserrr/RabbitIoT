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
}

Measurement::~Measurement() {
}

void Measurement::updateValue(const BaseData& value) {
    _log.log(LOGLEVEL_TRACE, "%s updateValue: %s - currentValue: %s", _id, value.stringValue(), _data ? _data->stringValue() : "NULL");
    if (!_data || !_data->equals(value)) { 
        if (_data) delete _data;
        _data = value.copy();
        updateTimestamp();
        setUpdated();
        _log.log(LOGLEVEL_TRACE, "%s updateValue - updated", _id);
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

void Measurement::updateTimestamp(unsigned long timestamp) {
    _lastUpdate = timestamp == 0 ? millis() : timestamp;
}
bool Measurement::intervalExceeded(unsigned long interval) const {
    return interval > 0 && millis() - _lastPublish > interval; 
}
