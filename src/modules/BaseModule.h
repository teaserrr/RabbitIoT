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

/**
 * A configurable module that can hold multiple measurements.
 * 
 * This class can be subclassed to provide more specific functionality.
 * Subclasses can override setup() and loopInner() to execute specific setup and runtime logic.
 * 
 * Another approach is to create an instance of this base class and provide pointers to the setup and loop
 * functions. The setup function will be called when setup() is called. The loop function will be called
 * when loop() is called and the loopDelay interval has passed.
 * 
 * A module can be enabled (default) or disabled by calling setEnabled(). When disabled, the loop logic is 
 * not executed.
 */
class BaseModule {
    public:
        /**
         * Creates a module without setup and loop functions.
         * Parameters:
         * - id: used to identify the module. Keep the id fairly short and concise.
         * - loopDelay: interval in milliseconds between consequent executions of the loop logic.
         * - mqttPath: (base) path of MQTT topic(s) which may be used by subclasses to determine the MQTT path for their
         *             measurement(s). 
         */
        BaseModule(const char* id, unsigned long loopDelay, const char* mqttPath);
        /**
         * Creates a module with setup and loop functions.
         * Parameters:
         * (see above)
         * - setupFunc: pointer to a setup function, to be called when setup() is called.
         * - loopFunc: pointer to a loop function, to be called when loop() is called.
         */
        BaseModule(const char* id, unsigned long loopDelay, const char* mqttPath, ModuleSetup setupFunc, ModuleLoop loopFunc);
        ~BaseModule();

        const char* getId() const;

        // Executes the setup logic. The default implementation calls setupFunc. It can be overridden in subclasses.
        // This is called when setup() is called on the RabbitIoT instance this module belongs to.
        virtual void setup();

        // Executes the loop logic. Checks shouldLoop() and calls loopFunc and loopInner(), in this order. 
        // This is called when loop() is called on the RabbitIoT instance this module belongs to.
        void loop();
        
        // Gets whether the module is enabled.
        bool isEnabled() const;

        // Enables or disables the module.
        void setEnabled(bool enabled);

        // Sets the logger. This is called when a module is added to a RabbitIoT instance.
        void setLogger(const Logger& logger);

        // Adds a measurement. Call this before setup.
        void addMeasurement(Measurement* measurement);

        // Gets all measurements of this module.
        Measurement** getMeasurements() const;

    protected:
        // Determines whether the loop logic should be executed.
        // Returns true if the module is enabled and the loopDelay interval has passed.
        bool shouldLoop() const;

         // Is called when loop() is called, only if shouldLoop() return true. The default implementation does nothing,
         // override in subclasses.
        virtual void loopInner();

        const char* getMqttPath() const;

        // Gets the measurement with the specified id. Returns NULL when this module has no measurement with this id.
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
