#ifndef BaseModule_h
#define BaseModule_h

#include <WString.h>

class BaseModule;

typedef void (*ModuleSetup) (const BaseModule* module);
typedef void (*ModuleLoop) (const BaseModule* module);

class BaseModule {
    public:
        BaseModule(String id);
        BaseModule(String id, ModuleSetup setupFunc, ModuleLoop loopFunc);
        ~BaseModule();

        void setup();
        void loop();

        const String getId() const;
        bool isEnabled() const;
        void setEnabled(bool enabled);

    protected:
        

    private:
        String _id;
        bool _enabled;
        ModuleSetup _setupFunc;
        ModuleLoop _loopFunc;
};

#endif
