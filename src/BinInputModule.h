#ifndef BinInputModule_h
#define BinInputModule_h

#include <BaseModule.h>

class BinInputModule : public BaseModule {
    public:
        BinInputModule(const char* id, uint8_t pin, unsigned long loopDelay = 1000, const char* mqttPath = NULL);
        ~BinInputModule();

        virtual void setup();

    protected:
        virtual void loopInner();
        
    private:
        Measurement* _state;
        uint8_t _pin;
        const char* _mqttPath;
};

#endif
