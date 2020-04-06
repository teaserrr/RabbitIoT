#ifndef RabbitIoT_h
#define RabbitIoT_h

#include <WString.h>
#include "Logger.h"
#include "BaseModule.h"

#define MAX_MODULES 16

class RabbitIot {
    public:
        RabbitIot(const Logger& logger = Logger());
        ~RabbitIot();

        void addModule(BaseModule* module);

        void setup();
        void loop();
    
    private:
        Logger _logger;
        BaseModule* _modules[MAX_MODULES];
        int moduleCount;
};

#endif
