#ifndef Logger_h
#define Logger_h

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

#ifndef LOGBUF_SIZE
#define LOGBUF_SIZE 1024
#endif

class Logger {
    public:
        Logger(int logLevel = DEFAULT_LOGLEVEL, int baudRate = DEFAULT_BAUDRATE);

        void trace(const char* message);
        void debug(const char* message);
        void info(const char* message);
        void warning(const char* message);
        void error(const char* message);
        void log(int level, const char* message, ...);
    
    private:
        int _logLevel;
        char _logbuf[LOGBUF_SIZE];
};

#endif
