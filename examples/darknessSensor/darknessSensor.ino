#include <RabbitIoT.h>
#include <modules/DarknessSensorModule.h>

#define ANALOG_PIN 17

Logger logger(LOGLEVEL_TRACE);
RabbitIot rabbit("testRabbit", logger);

void setup() {
    DarknessSensorModule* module = new DarknessSensorModule("darkness", ANALOG_PIN, 3000, 400, 200, "sensors/darkness/");
    rabbit.addModule(module);
    rabbit.setup();
}

void loop() {
    rabbit.loop();
    delay(100);
}
