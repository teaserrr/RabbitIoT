#ifndef MqttClient_h
#define MqttClient_h

#include <WString.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "Logger.h"

class MqttClient {
    public:
        MqttClient(const String& clientId, const Logger& logger = Logger());
        ~MqttClient();

        void setup();
        void loop();
        void publish(const String& topic, const String& payload);

        static MqttClient* _instance;

        void onMessageReceived(const String& topic, byte* payload, unsigned int length);

    private:
        bool reconnect();
        void createSubscriptions();

        Logger _logger;
        WiFiClient* _wifiClient;
        PubSubClient* _pubSubClient;
        String _clientId;
        unsigned long _lastMqttReconnectAttempt = 0;
};

#endif
