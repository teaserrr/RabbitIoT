#include "Logger.h"
#include <Arduino.h>

Logger::Logger(int logLevel, int baudRate) {
    _logLevel = logLevel;
    Serial.begin(baudRate);
}

void Logger::debug(const String& message) {
    log(LOGLEVEL_DEBUG, message);
}

void Logger::info(const String& message) {
    log(LOGLEVEL_INFO, message);
}

void Logger::warning(const String& message) {
    log(LOGLEVEL_WARNING, message);
}

void Logger::error(const String& message) {
    log(LOGLEVEL_ERROR, message);
}

void Logger::log(int level, const String& message) {
    if (level < _logLevel)
        return;

    const char *levelChar;
    switch (level) {
        case LOGLEVEL_DEBUG:
            levelChar = "DBG";
            break;
        case LOGLEVEL_INFO:
            levelChar = "INF";
            break;
        case LOGLEVEL_WARNING:
            levelChar = "WRN";
            break;
        case LOGLEVEL_ERROR:
            levelChar = "ERR";
            break;
    }
    char buffer[512];
    sprintf(buffer, "[%u] [%s] %s", millis(), levelChar, message.c_str());
    Serial.println(buffer);
}
