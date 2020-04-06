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
}

BaseModule::~BaseModule() {
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