#ifndef Logger_h
#define Logger_h

#include <WString.h>

#define LOGLEVEL_TRACE 10
#define LOGLEVEL_DEBUG 20
#define LOGLEVEL_INFO 30
#define LOGLEVEL_WARNING 40
#define LOGLEVEL_ERROR 50

#ifndef DEFAULT_LOGLEVEL
#define DEFAULT_LOGLEVEL LOGLEVEL_WARNING
#endif

#ifndef DEFAULT_BAUDRATE
#define DEFAULT_BAUDRATE 115200
#endif

class Logger {
    public:
        Logger(int logLevel = DEFAULT_LOGLEVEL, int baudRate = DEFAULT_BAUDRATE);

        void trace(const String& message);
        void debug(const String& message);
        void info(const String& message);
        void warning(const String& message);
        void error(const String& message);
    
    private:
        void log(int level, const String& message);

        int _logLevel;
};

#endif
