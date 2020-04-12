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

    protected:
        Measurement(const String& id, const String& description="", const String& unit="", const String& mqttTopic="") {
            _id = id;
            _description = description;
            _unit = unit;
            _mqttTopic = mqttTopic;
        }

        void updateTimestamp(unsigned long timestamp=0) {
            _lastUpdate = timestamp == 0 ? millis() : timestamp;
        }

    private:
        String _id;
        String _description;
        String _unit;
        String _mqttTopic;

        unsigned long _lastUpdate;
};

class FloatMeasurement : public Measurement {
    public:        
        FloatMeasurement(const String& id, const String& description="", const String& unit="", const String& mqttTopic="") :
            Measurement(id, description, unit, mqttTopic) {}

        virtual const String getStringValue() const {
            return String(_lastValue, 2);
        }

        void updateValue(float value) {
            _lastValue = value;
            updateTimestamp();
        }

    private:
        float _lastValue;
};

#endif
