#ifndef RabbitIoT_h
#define RabbitIoT_h

#include <WString.h>
#include <PubSubClient.h>
#include "Logger.h"
#include "BaseModule.h"
#include "Measurement.h"
#include "MqttClient.h"
#include "WebServer.h"

#ifndef MAX_MODULES
#define MAX_MODULES 16
#endif

class RabbitIot {
    public:
        RabbitIot(const char* deviceName, const Logger& logger = Logger());
        ~RabbitIot();

        void addModule(BaseModule* module);

        void setup();
        void loop();
    
        const char* getDeviceName() const { return _deviceName; }
        
        Measurement** getMeasurements() const;

    protected:
        void setupWifi();
        void setupMqtt();
        void setupWebServer();
        void publishMeasurements(Measurement** measurements);

    private:
        Logger _logger;
        MqttClient* _mqttClient;
        WebServer* _webServer;
        BaseModule* _modules[MAX_MODULES];
        const char* _deviceName;
        unsigned int _moduleCount;
};

#endif
