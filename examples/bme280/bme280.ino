#include <RabbitIoT.h>
#include <Bme280Module.h>

Logger logger(LOGLEVEL_DEBUG);
RabbitIot rabbit(logger);

void setup() {
    delay(3000);
    Bme280Module* module = new Bme280Module();
    rabbit.addModule(module);
    rabbit.setup();
}

void loop() {
    rabbit.loop();
}
