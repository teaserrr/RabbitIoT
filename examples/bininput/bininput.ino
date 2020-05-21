#include <RabbitIoT.h>
#include <modules/BinInputModule.h>

#define INPUT_PIN 14 // D5

Logger logger(LOGLEVEL_TRACE);
RabbitIot rabbit("testRabbit", "192.168.0.180", logger);

void setup() {
    delay(3000);
    BinInputModule* module = new BinInputModule("BinInPut", INPUT_PIN, 1000, "sensors/input/");
    rabbit.addModule(module);
    rabbit.setup();
}

void loop() {
    rabbit.loop();
    delay(100);
}
