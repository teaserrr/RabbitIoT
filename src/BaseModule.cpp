#include "BaseModule.h"

BaseModule::BaseModule(String id)
    : BaseModule(id, NULL, NULL) {
}

BaseModule::BaseModule(String id, ModuleSetup setupFunc, ModuleLoop loopFunc) {
    _id = id;
    _enabled = true;
    _setupFunc = setupFunc;
    _loopFunc = loopFunc;
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

void BaseModule::setup() {
    if (_setupFunc != NULL)
        _setupFunc(this);
}

void BaseModule::loop() {
    if (!isEnabled())
        return;
    
    if (_loopFunc != NULL)
        _loopFunc(this);
}
