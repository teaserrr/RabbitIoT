#include <RabbitIoT.h>
#include <modules/Bme280Module.h>

Logger logger(LOGLEVEL_DEBUG);
RabbitIot rabbit("testRabbit", logger);

void setup() {
    delay(3000);
    Bme280Module* module = new Bme280Module("BME280", 60000, "sensors/env/");
    rabbit.addModule(module);
    rabbit.setup();
}

void loop() {
    rabbit.loop();
    delay(500);
}
