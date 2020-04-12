#ifndef RabbitIoT_h
#define RabbitIoT_h

#include <WString.h>
#include "Logger.h"
#include "BaseModule.h"
#include "Measurement.h"

#ifndef MAX_MODULES
#define MAX_MODULES 16
#endif
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
        unsigned int _moduleCount;
};

#endif
