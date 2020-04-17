#include "RabbitIoT.h"
#include <WiFiManager.h>

RabbitIot::RabbitIot(const String& deviceName, const Logger& logger) {
    _deviceName = deviceName;
    _logger = logger;
    _moduleCount = 0;
    _mqttClient = NULL;
}

RabbitIot::~RabbitIot() {
    if (_mqttClient != NULL)
        delete _mqttClient;
    for (int i = 0; i < _moduleCount; i++)
        delete _modules[i];
}

void RabbitIot::addModule(BaseModule* module){
    _moduleCount++;
    if (_moduleCount > MAX_MODULES)
        return;

    _modules[_moduleCount-1] = module;
    module->setLogger(_logger);
}

void RabbitIot::setup() {
    setupWifi();
    setupMqtt();

    for (int i = 0; i < _moduleCount; i++)
        _modules[i]->setup();
}

void RabbitIot::loop() {
    _mqttClient->loop();

    for (int i = 0; i < _moduleCount; i++) {
        _logger.log(LOGLEVEL_TRACE, "loop: %s", _modules[i]->getId().c_str());
        _modules[i]->loop();
        _logger.log(LOGLEVEL_TRACE, "publish: %s", _modules[i]->getId().c_str());
        publishMeasurements(_modules[i]->getMeasurements());
    }
}

void RabbitIot::setupWifi() {
    _logger.info("Starting WiFiManager autoconnect...");
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(false);
    wifiManager.autoConnect();
    //wifiManager.startConfigPortal("OnDemandAP");
    //_logger.info("Connected to: " + WiFi.SSID());
    //_logger.info("IP address: " + WiFi.localIP().toString());
}

void RabbitIot::setupMqtt()
{
    _logger.debug("Setting up MQTT client...");
    _mqttClient = new MqttClient(_deviceName, _logger);
    _mqttClient->setup();
}

void RabbitIot::publishMeasurements(Measurement** measurements) {
    int i = 0;
    while (i < MAX_MEASUREMENTS && measurements[i]) {
        if (measurements[i]->shouldPublish()) {
            _logger.log(LOGLEVEL_DEBUG, "Publish measurement: %s value: %s", measurements[i]->getId().c_str(), measurements[i]->getStringValue().c_str());
            _mqttClient->publish(measurements[i]->getMqttTopic(), measurements[i]->getStringValue());
            measurements[i]->setPublished();
        }
        i++;
    }
}