#include "MqttClient.h"

MqttClient* MqttClient::_instance;

void onMqttMessageReceived(char* topic, byte* payload, unsigned int length) {
    MqttClient::_instance->onMessageReceived(topic, payload, length);
}

MqttClient::MqttClient(const String& clientId, const Logger& logger) {
    _logger = logger;
    _clientId = clientId;
    _pubSubClient = NULL;
    _wifiClient = NULL;
    _lastMqttReconnectAttempt = 0;
    _instance = this;
}

MqttClient::~MqttClient() {
    if (_pubSubClient != NULL) {
        delete _pubSubClient;
        delete _wifiClient;
    }
}

void MqttClient::setup(ConfigManager* configManager) {
    _wifiClient = new WiFiClient();
    _pubSubClient = new PubSubClient(*_wifiClient);
    _logger.debug("MQTT server: 192.168.0.180");
    _pubSubClient->setServer("192.168.0.180", 1883);
    _pubSubClient->setCallback(onMqttMessageReceived);
    _configManager = configManager;
}

void MqttClient::loop()
{
  if (!_pubSubClient->connected()) 
  {
    if (_lastMqttReconnectAttempt == 0)
      _logger.warning("MQTT disconnected");
      
    if (millis() - _lastMqttReconnectAttempt > 5000) 
    {
      _lastMqttReconnectAttempt = millis();
      _logger.debug("attempt MQTT reconnect");
      if (reconnect()) 
      {
        _lastMqttReconnectAttempt = 0;
      }
      else 
      {
        _logger.warning("MQTT reconnect failed");
      }
    }
  }
  _pubSubClient->loop();
}

void MqttClient::publish(const String& topic, const String& payload) {
    _pubSubClient->publish(topic.c_str(), payload.c_str());
}

bool MqttClient::reconnect() 
{
  if (_pubSubClient->connect(_clientId.c_str())) {
    _logger.info("MQTT connected");
    createSubscriptions();
  }
  return _pubSubClient->connected();
}

void MqttClient::createSubscriptions() {
}

void MqttClient::onMessageReceived(const char* topic, byte* payload, unsigned int length) {
    char* cPayload = new char[length+1];
    strncpy(cPayload, (char*)payload, length);
    _logger.log(LOGLEVEL_INFO, "Received MQTT message - topic: %s - length: %u - payload: %s", topic, length, cPayload);
    
    ConfigParameter* parameter = _configManager->getParameterByMqttTopic(topic);
    if (!parameter) {
      _logger.warning("No configuration parameter found for topic");
      return;
    }
    _logger.debug("Update configuration parameter value");
    parameter->setValue(cPayload);
    _configManager->saveParameters();
}
