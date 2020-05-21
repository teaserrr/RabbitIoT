#include "RabbitIoT.h"
#include <WiFiManager.h>
#include <WString.h>

RabbitIot::RabbitIot(const char* deviceName, const Logger& logger) {
    _deviceName = deviceName;
    _logger = logger;
    _moduleCount = 0;
    _mqttClient = NULL;
    _webServer = NULL;
    _configManager = NULL;

    _logger.init();
}

RabbitIot::~RabbitIot() {
    if (_mqttClient != NULL) delete _mqttClient;
    if (_webServer != NULL) delete _webServer;
    if (_configManager != NULL) delete _configManager;

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
    for (int i = 0; i < _moduleCount; i++)
        _modules[i]->setup();

    setupConfiguration();
    setupMqtt();
    setupWebServer();
}

void RabbitIot::loop() {
    _mqttClient->loop();
    _webServer->loop();

    for (int i = 0; i < _moduleCount; i++) {
        _modules[i]->loop();
        publishMeasurements(_modules[i]->getMeasurements());
    }
}

//#define WIFI_CONFIG_PORTAL

void RabbitIot::setupWifi() {
    WiFiManager wifiManager;
#ifndef WIFI_CONFIG_PORTAL
    _logger.info(PSTR("Starting WiFiManager autoconnect..."));
    wifiManager.setDebugOutput(false);
    wifiManager.autoConnect();
#else
    _logger.info(PSTR("Starting WiFiManager config portal..."));
    wifiManager.startConfigPortal(PSTR("OnDemandAP"));
#endif
    _logger.log(LOGLEVEL_INFO, PSTR("Connected to: %s"), WiFi.SSID().c_str());
    _logger.log(LOGLEVEL_INFO, PSTR("IP address: %s"), WiFi.localIP().toString().c_str());
}

void RabbitIot::setupMqtt()
{
    _logger.debug(PSTR("Setting up MQTT client..."));
    _mqttClient = new MqttClient(_deviceName, _logger);
    _mqttClient->setup(_configManager);
}

void RabbitIot::setupWebServer() {
    _logger.debug(PSTR("Setting up WebServer..."));
    _webServer = new WebServer(_logger);
    _webServer->setup(getDeviceName(), getMeasurements());
}

void RabbitIot::setupConfiguration() {
    _logger.debug(PSTR("Setting up configuration..."));
    _configManager = new ConfigManager(_logger);
    _configManager->setup(getConfigParameters());
    _configManager->loadParameters();
}

void RabbitIot::publishMeasurements(Measurement** measurements) {
    int i = 0;
    while (i < MAX_MEASUREMENTS && measurements[i]) {
        if (measurements[i]->shouldPublish()) {
            _logger.log(LOGLEVEL_DEBUG, PSTR("Publish measurement: %s value: %s"), measurements[i]->getId(), measurements[i]->getStringValue());
            _mqttClient->publish(measurements[i]->getMqttTopic(), measurements[i]->getStringValue());
            measurements[i]->setPublished();
        }
        i++;
    }
}

Measurement** RabbitIot::getMeasurements() const {
    Measurement** measurements = new Measurement*[_moduleCount * MAX_MEASUREMENTS + 1];
    int index = 0;
    for (int i = 0; i < _moduleCount; i++) {
        Measurement** moduleMeasurements = _modules[i]->getMeasurements();
        int j = 0;
        while (j < MAX_MEASUREMENTS && moduleMeasurements[j]) {
            measurements[index++] = moduleMeasurements[j];
            j++;
        }
    }
    measurements[index] = NULL;
    return measurements;
}

ConfigParameter** RabbitIot::getConfigParameters() const {
    ConfigParameter** parameters = new ConfigParameter*[_moduleCount * MAX_CONFIG_PARAMETERS + 1];
    int index = 0;
    for (int i = 0; i < _moduleCount; i++) {
        ConfigParameter** moduleParameters = _modules[i]->getConfigParameters();
        int j = 0;
        while (j < MAX_CONFIG_PARAMETERS && moduleParameters[j]) {
            parameters[index++] = moduleParameters[j];
            j++;
        }
    }
    parameters[index] = NULL;
    return parameters;
}

