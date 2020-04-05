#ifndef RabbitIoT_h
#define RabbitIoT_h

#include <WString.h>
#include <BaseModule.h>

#define MAX_MODULES 16

class RabbitIot {
    public:
        RabbitIot();
        ~RabbitIot();

        void addModule(BaseModule* module);

        void setup();
        void loop();
    
    private:
        BaseModule* _modules[MAX_MODULES];
        int moduleCount;
};

#endif