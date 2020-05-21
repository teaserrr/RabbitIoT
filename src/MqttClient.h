#ifndef MqttClient_h
#define MqttClient_h

#include <PubSubClient.h>
#include <WiFiClient.h>
#include "Logger.h"
#include "ConfigManager.h"

class MqttClient {
    public:
        MqttClient(const char* clientId, const Logger& logger);
        ~MqttClient();

        void setup(ConfigManager* configManager);
        void loop();
        void publish(const char* topic, const char* payload);

        static MqttClient* _instance;

        void onMessageReceived(const char* topic, byte* payload, unsigned int length);

    private:
        bool reconnect();
        void createSubscriptions();

        Logger _logger;
        WiFiClient* _wifiClient;
        PubSubClient* _pubSubClient;
        ConfigManager* _configManager;
        const char* _clientId;
        unsigned long _lastMqttReconnectAttempt = 0;
};

#endif
