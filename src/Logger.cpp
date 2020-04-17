#include "Logger.h"
#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

#define TIMESTAMP_START 1
#define TIMESTAMP_LEN 10
#define LVL_START 14
#define LVL_LEN 3
#define MSG_START 19
#define MSG_LEN LOGBUF_SIZE - MSG_START

void copyLevelString(char* bufPtr, int level) {
    switch (level) {
        case LOGLEVEL_TRACE:
            strncpy(bufPtr, "TRC", LVL_LEN);
            break;
        case LOGLEVEL_DEBUG:
            strncpy(bufPtr, "DBG", LVL_LEN);
            break;
        case LOGLEVEL_INFO:
            strncpy(bufPtr, "INF", LVL_LEN);
            break;
        case LOGLEVEL_WARNING:
            strncpy(bufPtr, "WRN", LVL_LEN);
            break;
        case LOGLEVEL_ERROR:
            strncpy(bufPtr, "ERR", LVL_LEN);
            break;
        default:
            strncpy(bufPtr, "___", LVL_LEN);
            break;
    }
}

Logger::Logger(int logLevel, int baudRate) {
    _logLevel = logLevel;
    Serial.begin(baudRate);
  
    // 01234567890123456789
    // [1234567890] [LVL] messsage
    _logbuf[0] = '[';
    _logbuf[11] = ']';
    _logbuf[12] = ' ';
    _logbuf[13] = '[';
    _logbuf[17] = ']';
    _logbuf[18] = ' ';
    _logbuf[LOGBUF_SIZE-1] = 0;
}

void Logger::trace(const char* message) {
    log(LOGLEVEL_TRACE, message);
}

void Logger::debug(const char* message) {
    log(LOGLEVEL_DEBUG, message);
}

void Logger::info(const char* message) {
    log(LOGLEVEL_INFO, message);
}

void Logger::warning(const char* message) {
    log(LOGLEVEL_WARNING, message);
}

void Logger::error(const char* message) {
    log(LOGLEVEL_ERROR, message);
}

void Logger::log(int level, const char* message, ...) {
    if (level < _logLevel)
        return;

    char* bufPtr = _logbuf + TIMESTAMP_START;
    sprintf(bufPtr, "%0*u", TIMESTAMP_LEN, millis());
    _logbuf[TIMESTAMP_LEN+1] = ']'; // overwritten with \0 by sprintf

    bufPtr = _logbuf + LVL_START;
    copyLevelString(bufPtr, level);
    
    bufPtr = _logbuf + MSG_START;

    va_list args;
    va_start(args, message);
    vsiprintf(bufPtr, message, args);
    va_end(args);

    Serial.println(_logbuf);
}
