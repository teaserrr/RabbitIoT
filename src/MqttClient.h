#ifndef MqttClient_h
#define MqttClient_h

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "Logger.h"
#include "ConfigManager.h"

class MqttClient {
    public:
        MqttClient(const Logger& logger, const char* clientId, const char* host, uint16_t port=1883);
        ~MqttClient();

        void setup();
        void setConfigManager(ConfigManager* configManager);
        void loop();
        void publish(const char* topic, const char* payload);

        static MqttClient* _instance;

        void onMessageReceived(const char* topic, byte* payload, unsigned int length);

    private:
        void createClient();
        bool reconnect();
        void createSubscriptions();

        Logger _logger;
        WiFiClient* _wifiClient;
        PubSubClient* _pubSubClient;
        ConfigManager* _configManager;
        ConfigParameter* _cHost;
        ConfigParameter* _cPort;
        const char* _clientId;
        const char* _host;
        uint16_t _port;
        unsigned long _lastMqttReconnectAttempt = 0;
};

#endif
