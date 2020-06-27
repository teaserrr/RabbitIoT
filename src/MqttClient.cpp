#include "MqttClient.h"
#include <functional>

MqttClient::MqttClient(const Logger& logger, const char* clientId, const char* host, uint16_t port) {
    _logger = logger;
    _clientId = clientId;
    _host = host;
    _port = port;
    _pubSubClient = NULL;
    _wifiClient = NULL;
    _lastMqttReconnectAttempt = 0;
}

MqttClient::~MqttClient() {
    if (_pubSubClient != NULL) {
        delete _pubSubClient;
        delete _wifiClient;
    }
}

void MqttClient::setup() {
    _cHost = new ConfigParameter("host", "MQTT host", _host, 32, NULL);
    _cPort = new ConfigParameter("port", "MQTT port", _port, NULL);
}

void MqttClient::setConfigManager(ConfigManager* configManager) {
    _configManager = configManager;
}

void MqttClient::createClient() {
    if (!strlen(_cHost->getValue()) || !_cPort->getIntValue())
        return;

    if (!_wifiClient)
        _wifiClient = new WiFiClient();
    
    if (_pubSubClient)
        delete _pubSubClient;

    _pubSubClient = new PubSubClient(*_wifiClient);
    _logger.log(LOGLEVEL_INFO, PSTR("MQTT server: %s - port: %u"), _cHost->getValue(), _cPort->getIntValue());
    _pubSubClient->setServer(_cHost->getValue(), _cPort->getIntValue());
    using namespace std::placeholders;
    _pubSubClient->setCallback(std::bind(&MqttClient::onMessageReceived, this, _1, _2, _3));
}

void MqttClient::loop()
{
    if (!_pubSubClient) createClient();
    if (!_pubSubClient) return;

    if (!_pubSubClient->connected()) 
    {
        if (_lastMqttReconnectAttempt == 0)
            _logger.warning(PSTR("MQTT disconnected"));

        if (millis() - _lastMqttReconnectAttempt > 5000) 
        {
            _lastMqttReconnectAttempt = millis();
            _logger.debug(PSTR("attempt MQTT reconnect"));
            if (reconnect()) 
            {
                _lastMqttReconnectAttempt = 0;
            }
            else 
            {
                _logger.warning(PSTR("MQTT reconnect failed"));
            }
        }
    }
    _pubSubClient->loop();
}

void MqttClient::publish(const char* topic, const char* payload) {
    if (!_pubSubClient) return;
    _pubSubClient->publish(topic, payload);
}

bool MqttClient::reconnect() 
{
    if (_pubSubClient->connect(_clientId)) {
        _logger.info(PSTR("MQTT connected"));
        createSubscriptions();
    }
    return _pubSubClient->connected();
}

void MqttClient::createSubscriptions() {
    ConfigParameter** parameters = _configManager->getParameters();
    int i = 0;
    while (parameters[i]) {
        _pubSubClient->subscribe(parameters[i]->getMqttTopic());
        i++;
    }
}

void MqttClient::onMessageReceived(const char* topic, byte* payload, unsigned int length) {
    char* cPayload = new char[length+1];
    strncpy(cPayload, (char*)payload, length);
    cPayload[length] = 0;

    _logger.log(LOGLEVEL_INFO, PSTR("Received MQTT message - topic: %s - length: %u - payload: %s"), topic, length, cPayload);
    
    ConfigParameter* parameter = _configManager->getParameterByMqttTopic(topic);
    if (!parameter) {
        _logger.warning(PSTR("No configuration parameter found for topic"));
        return;
    }
    _logger.debug(PSTR("Update configuration parameter value"));
    parameter->setValue(cPayload);
    _configManager->saveParameters();

    delete cPayload;
}
