#include "BaseModule.h"
#include <Arduino.h>

BaseModule::BaseModule(String id, unsigned long loopDelay)
    : BaseModule(id, loopDelay, NULL, NULL) {
}

BaseModule::BaseModule(String id, unsigned long loopDelay, ModuleSetup setupFunc, ModuleLoop loopFunc) {
    _id = id;
    _enabled = true;
    _setupFunc = setupFunc;
    _loopFunc = loopFunc;
    _loopDelay = loopDelay;
    _lastLoopTime = 0;
    _measurements = new Measurement*[MAX_MEASUREMENTS];
}

BaseModule::~BaseModule() {
    for (int i = 0; i < _measurementCount; i++)
        delete _measurements[i];
    delete _measurements;
}

const String BaseModule::getId() const {
    return _id;
}

bool BaseModule::isEnabled() const {
    return _enabled;
}

void BaseModule::setEnabled(bool enabled) {
    _enabled = enabled;
}

bool BaseModule::shouldLoop() const {
    return _enabled && millis() - _lastLoopTime > _loopDelay;
}

void BaseModule::setup() {
    if (_setupFunc != NULL)
        _setupFunc(this);
}

void BaseModule::loop() {
    if (!shouldLoop())
        return;
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
    if (_measurementCount > MAX_MEASUREMENTS)
        return;

    _measurements[_measurementCount-1] = measurement;
}

Measurement** BaseModule::getMeasurements() const {
    return _measurements;
}

Measurement* BaseModule::getMeasurement(const String& id) const {
    for (int i = 0; i < _measurementCount; i++) {
        if (_measurements[i]->getId().equals(id))
            return _measurements[i];
    }
    return NULL;
}
