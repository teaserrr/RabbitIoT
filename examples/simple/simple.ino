#include <RabbitIoT.h>
#include <Arduino.h>

RabbitIot rabbit("testRabbit");

void setup() {
    BaseModule* module = new BaseModule("testModule", 1000, &moduleSetup, &moduleLoop);
    rabbit.addModule(module);
    rabbit.setup();
}

void loop() {
    rabbit.loop();
}

void moduleSetup(const BaseModule* module) {
    Serial.print("setup ");
    Serial.println(module->getId());
}

void moduleLoop(const BaseModule* module) {
    Serial.print("loop ");
    Serial.println(module->getId());
}
