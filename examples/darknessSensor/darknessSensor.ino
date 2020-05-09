#include <RabbitIoT.h>
#include <modules/DarknessSensorModule.h>

Logger logger(LOGLEVEL_TRACE);
RabbitIot rabbit("testRabbit", logger);

void setup() {
    delay(3000);
    DarknessSensorModule* module = new DarknessSensorModule("darkness", 17, 1000, 400, 200, "sensors/darkness");
    rabbit.addModule(module);
    rabbit.setup();
}

void loop() {
    rabbit.loop();
    delay(100);
}
