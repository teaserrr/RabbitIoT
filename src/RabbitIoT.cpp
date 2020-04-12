#include "RabbitIoT.h"

RabbitIot::RabbitIot(const Logger& logger) {
    _logger = logger;
    _moduleCount = 0;    
}

RabbitIot::~RabbitIot() {
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
    for (int i = 0; i < _moduleCount; i++)
        _modules[i]->setup();
}

void RabbitIot::loop() {
    for (int i = 0; i < _moduleCount; i++)
        _modules[i]->loop();
}
