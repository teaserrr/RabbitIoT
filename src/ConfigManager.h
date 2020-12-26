#ifndef ConfigManager_h
#define ConfigManager_h

#include "Logger.h"
#include "ConfigParameter.h"
#include <ArduinoJson.h>

#ifndef CONFIG_FILE_PATH
#define CONFIG_FILE_PATH "/configuration.json"
#endif

class ConfigManager {
    public:
        ConfigManager(const Logger& logger);
        ~ConfigManager();

        void setup(ConfigParameter** parameters);

        void loadParameters();
        size_t saveParameters();

        ConfigParameter** getParameters() { return _parameters; }
        ConfigParameter* getParameterByMqttTopic(const char* mqttTopic);

    private:
        void loadParameters(const DynamicJsonDocument& document); 
        int calculateJsonSize(ConfigParameter** parameters);

        Logger _log;
        ConfigParameter** _parameters;
        int _jsonSize;
};

#endif
