#ifndef Measurement_h
#define Measurement_h

#include "Logger.h"
#include <Arduino.h>
#include <WString.h>

class Measurement {
    public:
        ~Measurement() { }

        const String getId() const { return _id; }
        const String getDescription() const { return _description; }
        const String getUnit() const { return _unit; }
        const String getMqttTopic() const { return _mqttTopic; }
    
        virtual const String getStringValue() const = 0;

        unsigned long getLastUpdated() const { return _lastUpdate; }

        bool shouldPublish() {
            return _mqttTopic.length() > 0 && 
                    !_neverUpdated &&
                    (
                        (_isUpdated && (intervalExceeded(_maxPublishInterval) || _maxPublishInterval == 0))
                        || intervalExceeded(_minPublishInterval)
                    );
        }

        void setPublished() {
            if (!_isUpdated)
                updateTimestamp();
            resetUpdated();
        }

    protected:
        Measurement(const String& id, const String& description="", const String& unit="", const String& mqttTopic="", unsigned long minPublishInterval = 0, unsigned long maxPublishInterval = 0) {
            _id = id;
            _description = description;
            _unit = unit;
            _mqttTopic = mqttTopic;
            _minPublishInterval = minPublishInterval;
            _maxPublishInterval = maxPublishInterval;
            _isUpdated = false;
            _lastUpdate = 0;
            _neverUpdated = true;
        }

        void updateTimestamp(unsigned long timestamp=0) {
            _lastUpdate = timestamp == 0 ? millis() : timestamp;
        }

        void setUpdated() { _isUpdated = true; _neverUpdated = false; }
        void resetUpdated() { _isUpdated = false; }
        
        bool _neverUpdated;

    private:
        bool intervalExceeded(unsigned long interval) {
            return interval > 0 && millis() - _lastUpdate > interval; 
        }

        String _id;
        String _description;
        String _unit;
        String _mqttTopic;
        unsigned long _minPublishInterval;
        unsigned long _maxPublishInterval;

        unsigned long _lastUpdate;
        bool _isUpdated;
};

class FloatMeasurement : public Measurement {
    public:        
        FloatMeasurement(const String& id, const String& description="", const String& unit="", const String& mqttTopic="") :
            Measurement(id, description, unit, mqttTopic) {}

        virtual const String getStringValue() const {
            return String(_lastValue, 2);
        }

        void updateValue(float value) {
            if (_lastValue != value || _neverUpdated) { 
                _lastValue = value;
                updateTimestamp();
                setUpdated();
            }
        }

    private:
        float _lastValue;
};

#endif
