#ifndef RabbitIoT_h
#define RabbitIoT_h

#include <WString.h>
#include <PubSubClient.h>
#include "Logger.h"
#include "modules/BaseModule.h"
#include "Measurement.h"
#include "MqttClient.h"
#include "WebServer.h"
#include "ConfigManager.h"
#include "ConfigParameter.h"

#ifndef MAX_MODULES
#define MAX_MODULES 16
#endif

/**
 * Represents an IoT device.
 * 
 * Simple HOWTO:
 *      Start by adding one or more modules in your sketches setup().
 *      Then, call setup() on this class.
 *  
 *      In your sketches loop() function, call loop() on this class.
 *      That's it!
 * 
 */
class RabbitIot {
    public:
        /**
         * Creates a rabbit!
         * 
         * Parameters:
         * - deviceName: the name of your IoT device. This is displayed in the web interface and is used by the MQTT client.
         * - logger: the default logger implementation logs to Serial output with log level WARNING.
         */
        RabbitIot(const char* deviceName, const Logger& logger = Logger());
        ~RabbitIot();

        // call this to add modules to the device, BEFORE you call setup()
        void addModule(BaseModule* module);

        // call this in the sketches setup()
        void setup();

        // call this in the sketches loop()
        void loop();
    
        const char* getDeviceName() const { return _deviceName; }

    protected:
        void setupWifi();
        void setupMqtt();
        void setupWebServer();
        void setupConfiguration();
        
        // Gets an array of all measurements of all modules.
        // The size of the array may be larger than the amount of measurements. The last item in the array is always NULL.
        // It's the responsibility of the caller to delete the resulting array.
        Measurement** getMeasurements() const;

        // Gets an array of all configuration parameters of all modules.
        // The size of the array may be larger than the amount of parameters. The last item in the array is always NULL.
        // It's the responsibility of the caller to delete the resulting array.
        ConfigParameter** getConfigParameters() const;

        void publishMeasurements(Measurement** measurements);

    private:
        Logger _logger;
        MqttClient* _mqttClient;
        WebServer* _webServer;
        ConfigManager* _configManager;
        BaseModule* _modules[MAX_MODULES];
        const char* _deviceName;
        unsigned int _moduleCount;
};

#endif
