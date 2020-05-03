#ifndef Measurement_h
#define Measurement_h

#include "Logger.h"
#include "Data.h"

#define I_1MINUTE 60000
#define I_1HOUR 3600000
#define I_1DAY 86400000

/**
 * Represents a single sensor measurement, such as an on/off state, temperature or light value.
 * 
 * This class holds all information and metadata about this measurement and contains the logic to determine whether and when the data was last
 * updated, whether it should be published over MQTT etc.
 */
class Measurement {
    public:
        /**
         * Parameters:
         * - id: used to identify the measurement. Keep the id fairly short and concise.
         * - description: human readable description of the measurement. Displayed in the web interface.
         * - unit: unit of the measurement. E.g. "°C", "lux", or just an empty string. Displayed in the web interface.
         * - mqttTopic: path of the MQTT topic to publish the value of the measurement. E.g. "sensors/temperature". When NULL, the measurement
         *              will not be published.
         * - minPublishInterval: determines the time interval that the value must be published 'at minimum'. If the value hasn't changed for some
         *                       time, this parameter may trigger additional publishes of the last recorded value. E.g. if set to 3600000ms (1 hour),
         *                       the value will be published at least each hour if the value doesn't change. If the value does change, it will be
         *                       published more often. Value in milliseconds. If set to 0, the parameter is ignored.
         * - maxPublishInterval: determines the time interval that the value is allowed to be published 'at maximum'. If a measurement changes very
         *                       often, this parameter is used to limit the amount of times the value is published. E.g. if set to 1000ms, the value
         *                       will be published at maximum each second if the measurement changes more often than that. If the measurement changes 
         *                       less often though, it will be published only when it has changed.
         */
        Measurement(const Logger& logger, const char* id, const char* description = NULL, const char* unit = NULL, const char* mqttTopic = NULL, 
                    unsigned long minPublishInterval = 0, unsigned long maxPublishInterval = 0);
        ~Measurement();

        const char* getId() const { return _id; }
        const char* getDescription() const { return _description; }
        const char* getUnit() const { return _unit; }
        const char* getMqttTopic() const { return _mqttTopic; }
    
        // Updates the measurement with a new value and marks it as updated.
        // The value parameter is copied so the original object may be discarded.
        void updateValue(const BaseData& value);

        // Gets the string representation of the last recorded value, or the empty string if no data was set yet in this measurement.
        virtual const char* getStringValue() { return _data ? _data->stringValue() : ""; }

        // Gets the data object. May be NULL.
        const BaseData* data() const { return _data; }

        // Gets the timestamp of the last update. Returns 0 if it has never been updated.
        unsigned long getLastUpdated() const { return _lastUpdate; }

        // Gets whether the data should be published over MQTT.
        // Returns false if the MQTT topic is NULL or if the data has never been updated.
        // Returns true if the data was updated since the last publish, but:
        // - if maxPublishInterval is > 0, it will return false if the last publish time is less ms ago than this interval
        // - if minPublishInterval is > 0, it will return true if the last publish time is more ms ago than this interval even if the
        //   data was not update since the last publish time.
        bool shouldPublish() const;

        // Marks the value as published over MQTT.
        void setPublished();

    protected:
        void updateTimestamp(unsigned long timestamp=0);
        void setUpdated() { _isUpdated = true; }
        void resetUpdated();
        bool intervalExceeded(unsigned long interval) const;

    private:
        Logger _log;
        const BaseData* _data;
        const char* _id;
        const char* _description;
        const char* _unit;
        const char* _mqttTopic;
        unsigned long _minPublishInterval;
        unsigned long _maxPublishInterval;

        unsigned long _lastUpdate;
        unsigned long _lastPublish;
        bool _isUpdated;
        bool _firstTimePublish;
};

#endif
