#include "ConfigManager.h"

#include <FS.h>
#include <ArduinoJson.h>

ConfigManager::ConfigManager(const Logger& logger) {
    _log = logger;
    _parameters = NULL;

	if (!SPIFFS.begin()) 
        _log.error("Failed to mount file system");
    else
        _log.debug("Mounted file system");
}

ConfigManager::~ConfigManager() {
    if (_parameters) delete _parameters;
}

void ConfigManager::setup(ConfigParameter** parameters) {
    _parameters = parameters;
    _jsonSize = calculateJsonSize(parameters);
}

void ConfigManager::loadParameters() {
    _log.info("Loading configuration parameters");

    if (!SPIFFS.exists(CONFIG_FILE_PATH)) {
        _log.info("Config file does not exist: " CONFIG_FILE_PATH);
        return;
    }

    _log.debug("  opening config file");
    File configFile = SPIFFS.open(CONFIG_FILE_PATH, "r");
    if (!configFile) {
        _log.error("Failed to open config file");
        return;
    }

    _log.debug("  deserializing json");
    int capacity = _jsonSize;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, configFile);
    while (error) {
        _log.error("Failed to deserialize json:");
        _log.error(error.c_str());
        if (error != DeserializationError::NoMemory || doc.capacity() == 0) // capacity 0 means allocation failed
            break;
        capacity += 128;
        doc = DynamicJsonDocument(capacity);
        error = deserializeJson(doc, configFile);
    }
    if (!error) {
        _log.debug("  deserialized json");
        loadParameters(doc);
    }
    configFile.close();    
}

void ConfigManager::loadParameters(const DynamicJsonDocument& document) {
    int i = 0;
    while (_parameters[i]) {
        const char* paramId = _parameters[i]->getId();
        const char* value = document[paramId];
        if (value) {
            _log.log(LOGLEVEL_INFO, "Init config parameter %s to value %s", paramId, value);
           _parameters[i]->setValue(value); 
        }
        i++;
    }
}

void ConfigManager::saveParameters() {
    _log.info("Saving configuration parameters");

    if (!_parameters[0])
        return; // no parameters

    DynamicJsonDocument document(_jsonSize);
    int i = 0;
    while (_parameters[i]) {
        _log.log(LOGLEVEL_DEBUG, "Save config parameter %s with value %s", _parameters[i]->getId(), _parameters[i]->getValue());
        document[_parameters[i]->getId()] = _parameters[i]->getValue();
        i++;
    }

    _log.debug("  opening config file");
    File configFile = SPIFFS.open(CONFIG_FILE_PATH, "w");
    if (!configFile) {
        _log.error("Failed to open config file for writing");
    }

    _log.debug("  serializing json");
    serializeJson(document, configFile);
    configFile.close();
}

ConfigParameter* ConfigManager::getParameterByMqttTopic(const char* mqttTopic) {
    int i = 0;
    while (_parameters[i]) {
        if (strcmp(mqttTopic, _parameters[i]->getMqttTopic()) == 0)
            return _parameters[i];
        i++;
    }
    _log.log(LOGLEVEL_WARNING, "Config parameter for topic %s not found", mqttTopic);
    return NULL;
}

int ConfigManager::calculateJsonSize(ConfigParameter** parameters) {
    int size = 0;
    int i = 0;
    while (parameters[i]) {
        size += strlen(parameters[i]->getId());
        size += parameters[i]->getMaxLength();
        i++;
    }
    size += 2*i; // null terminators, 2 for each parameter
    size += JSON_OBJECT_SIZE(i);
    return size;
}