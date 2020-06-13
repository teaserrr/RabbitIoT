#include <RabbitIoT.h>
#include <modules/BinInputModule.h>

#define INPUT_PIN 14 // D5
#define OUTPUT_PIN 12 // D6

Logger logger(LOGLEVEL_INFO);
RabbitIot rabbit("testRabbit", "192.168.0.180", logger);
BinInputModule module("BinInPut", INPUT_PIN, 100, "sensors/input/");
StateListener listener(&inputChanged);

void setup() {
    pinMode(OUTPUT_PIN, OUTPUT);
    rabbit.addModule(&module);
    rabbit.setup();
    module.addStateListener(&listener);
}


void loop() {
    rabbit.loop();
}

void inputChanged(const Measurement* measurement) {
    digitalWrite(OUTPUT_PIN, module.getState());
}