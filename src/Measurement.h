#ifndef Measurement_h
#define Measurement_h

#include "Logger.h"
#include "Data.h"

#define I_1MINUTE 60000
#define I_1HOUR 3600000
#define I_1DAY 86400000

class Measurement {
    public:
        Measurement(const Logger& logger, const char* id, const char* description = NULL, const char* unit = NULL, char* mqttTopic = NULL, 
                    unsigned long minPublishInterval = 0, unsigned long maxPublishInterval = 0);
        ~Measurement();

        const char* getId() const { return _id; }
        const char* getDescription() const { return _description; }
        const char* getUnit() const { return _unit; }
        const char* getMqttTopic() const { return _mqttTopic; }
    
        void updateValue(const BaseData& value);

        virtual const char* getStringValue() { return _data ? _data->stringValue() : ""; }
        const BaseData* data() const { return _data; }
        unsigned long getLastUpdated() const { return _lastUpdate; }
        bool shouldPublish() const;
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
