#ifndef BaseModule_h
#define BaseModule_h

#include <WString.h>

class BaseModule;

typedef void (*ModuleSetup) (const BaseModule* module);
typedef void (*ModuleLoop) (const BaseModule* module);

class BaseModule {
    public:
        BaseModule(String id, unsigned long loopDelay);
        BaseModule(String id, unsigned long loopDelay, ModuleSetup setupFunc, ModuleLoop loopFunc);
        ~BaseModule();

        virtual void setup();
        void loop();

        const String getId() const;
        bool isEnabled() const;
        void setEnabled(bool enabled);

    protected:
        bool shouldLoop() const;
        virtual void loopInner();

    private:
        String _id;
        bool _enabled;
        ModuleSetup _setupFunc;
        ModuleLoop _loopFunc;
        unsigned long _loopDelay;
        unsigned long _lastLoopTime;
};

#endif
