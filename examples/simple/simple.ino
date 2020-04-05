#include <RabbitIoT.h>
#include <Arduino.h>

RabbitIot rabbit;

void setup() {
    BaseModule* module = new BaseModule("test", &moduleSetup, &moduleLoop);
    rabbit.addModule(module);
    rabbit.setup();
}

void loop() {
    rabbit.loop();
}

void moduleSetup(const BaseModule* module) {
    Serial.begin(115200);
    Serial.print("setup ");
    Serial.println(module->getId());
}

void moduleLoop(const BaseModule* module) {
    Serial.print("loop ");
    Serial.println(module->getId());
}