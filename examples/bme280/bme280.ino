#include <RabbitIoT.h>
#include <Bme280Module.h>

RabbitIot rabbit;

void setup() {
    Bme280Module* module = new Bme280Module();
    rabbit.addModule(module);
    rabbit.setup();
}

void loop() {
    rabbit.loop();
}
