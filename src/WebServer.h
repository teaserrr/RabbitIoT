#ifndef WebServer_h
#define WebServer_h

#include "Logger.h"
#include "Measurement.h"
#include "ConfigManager.h"
#include <ESP8266WebServer.h>

class WebServer {
    public:
        WebServer(const Logger& logger, int port = 80);
        ~WebServer();

        typedef std::function<void(void)> THandlerFunction;
        void addUriHandler(const char *uri, THandlerFunction handler);
        void setup(const char* deviceName, Measurement** measurements, ConfigManager* configManager);
        void loop();

        void handleRoot();
        void handleConfiguration();
        void handleSave();

    protected:
        Logger _logger;

    private:
        Measurement** _measurements;
        ConfigManager* _configManager;
        ESP8266WebServer* _server;
        const char* _deviceName;
};

#endif
