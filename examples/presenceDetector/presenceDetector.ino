#include <RabbitIoT.h>
#include <modules/PresenceDetectorModule.h>

#define INPUT_PIN 14 // D5

Logger logger(LOGLEVEL_TRACE);
RabbitIot rabbit("testRabbitPD", "192.168.0.180", logger);
PresenceDetectorModule module("Presence", INPUT_PIN, "sensors/presence");

void setup() {
    rabbit.addModule(&module);
    rabbit.setup();
}

void loop() {
    rabbit.loop();
    delay(100);
}