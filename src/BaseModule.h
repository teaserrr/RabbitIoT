#ifndef BaseModule_h
#define BaseModule_h

#include "Logger.h"
#include "Measurement.h"

// maximum number of measurements for a single module
#ifndef MAX_MEASUREMENTS
#define MAX_MEASUREMENTS 5
#endif

class BaseModule;

typedef void (*ModuleSetup) (const BaseModule* module);
typedef void (*ModuleLoop) (const BaseModule* module);

class BaseModule {
    public:
        BaseModule(const char* id, unsigned long loopDelay, const char* mqttPath);
        BaseModule(const char* id, unsigned long loopDelay, const char* mqttPath, ModuleSetup setupFunc, ModuleLoop loopFunc);
        ~BaseModule();

        virtual void setup();
        void loop();

        const char* getId() const;
        bool isEnabled() const;
        void setEnabled(bool enabled);
        void setLogger(const Logger& logger);

        void addMeasurement(Measurement* measurement);
        Measurement** getMeasurements() const;

    protected:
        bool shouldLoop() const;
        virtual void loopInner();

        const char* getMqttPath() const;
        Measurement* getMeasurement(const char* id) const;
        Logger _log;

    private:
        const char* _id;
        const char* _mqttPath;
        bool _enabled;
        bool _firstTime;
        ModuleSetup _setupFunc;
        ModuleLoop _loopFunc;
        unsigned long _loopDelay;
        unsigned long _lastLoopTime;

        Measurement** _measurements;
        unsigned int _measurementCount;
};

char* concat(const char* str1, const char* str2);

#endif
