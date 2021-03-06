#include "BaseModule.h"
#include <Arduino.h>

BaseModule::BaseModule(const char* id, unsigned long loopDelay, const char* mqttPath)
    : BaseModule(id, loopDelay, mqttPath, NULL, NULL) {
}

BaseModule::BaseModule(const char* id, unsigned long loopDelay, const char* mqttPath, ModuleSetup setupFunc, ModuleLoop loopFunc) {
    _id = id;
    _mqttPath = mqttPath;
    _enabled = true;
    _firstTime = true;
    _setupFunc = setupFunc;
    _loopFunc = loopFunc;
    _loopDelay = loopDelay;
    _lastLoopTime = 0;
    _measurementCount = 0;
    _parameterCount = 0;
    _measurements = new Measurement*[MAX_MEASUREMENTS];
    _configParameters = new ConfigParameter*[MAX_CONFIG_PARAMETERS];
    for (int i = 0; i < MAX_MEASUREMENTS; i++)
        _measurements[i] = NULL;
    for (int i = 0; i < MAX_CONFIG_PARAMETERS; i++)
        _configParameters[i] = NULL;
}

BaseModule::~BaseModule() {
    for (int i = 0; i < _measurementCount; i++)
        if (_measurements[i]) delete _measurements[i];
    delete _measurements;
    for (int i = 0; i < _parameterCount; i++)
        if (_configParameters[i]) delete _configParameters[i];
    delete _configParameters;
}

const char* BaseModule::getId() const {
    return _id;
}

bool BaseModule::isEnabled() const {
    return _enabled;
}

const char* BaseModule::getMqttPath() const { 
    return _mqttPath; 
}

void BaseModule::setEnabled(bool enabled) {
    _enabled = enabled;
}

bool BaseModule::shouldLoop() const {
    return _enabled && (_firstTime || millis() - _lastLoopTime > _loopDelay);
}

void BaseModule::setup() {
    if (_setupFunc != NULL)
        _setupFunc(this);
}

void BaseModule::loop() {
    if (!shouldLoop())
        return;
    _firstTime = false;
    _lastLoopTime = millis();

    if (_loopFunc != NULL)
        _loopFunc(this);
    
    loopInner();
}

void BaseModule::loopInner() {
    // default: do nothing
}

void BaseModule::setLogger(const Logger &logger) {
    _log = logger;
}

void BaseModule::addMeasurement(Measurement* measurement) {
    _measurementCount++;
    if (_measurementCount > MAX_MEASUREMENTS) {
        _log.warning(PSTR("Maximum amount of measurements exceeded. Increase MAX_MEASUREMENTS to fix this."));
        return;
    }

    _measurements[_measurementCount-1] = measurement;
}

Measurement** BaseModule::getMeasurements() const {
    return _measurements;
}

Measurement* BaseModule::getMeasurement(const char* id) const {
    for (int i = 0; i < _measurementCount; i++) {
        if (strcmp(_measurements[i]->getId(), id) == 0)
            return _measurements[i];
    }
    return NULL;
}

void BaseModule::addConfigParameter(ConfigParameter* configParameter) {
    _parameterCount++;
    if (_parameterCount > MAX_CONFIG_PARAMETERS) {
        _log.warning(PSTR("Maximum amount of configuration parameters exceeded. Increase MAX_CONFIG_PARAMETERS to fix this."));
        return;
    }

    _configParameters[_parameterCount-1] = configParameter;
}

ConfigParameter** BaseModule::getConfigParameters() const {
    return _configParameters;
}

ConfigParameter* BaseModule::getConfigParameter(const char* id) const {
    for (int i = 0; i < _parameterCount; i++) {
        if (strcmp(_configParameters[i]->getId(), id) == 0)
            return _configParameters[i];
    }
    return NULL;
}

char* concat(const char* str1, const char* str2) {
    size_t len = strlen(str1) + strlen(str2);
    bool slash = str1[strlen(str1)-1] == '/' || str2[0] == '/';
    if (!slash) len++;
    char* buf = new char[len+1];
    strcpy(buf, str1);
    if (!slash) strcat(buf, "/");
    strcat(buf, str2);
    return buf;
}
