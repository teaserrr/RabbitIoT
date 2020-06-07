#include "WebServer.h"
#include "HtmlDefines.h"

WebServer::WebServer(const Logger& logger, int port) {
    _logger = logger;
    _server = new ESP8266WebServer(port);
    _measurements = NULL;
}

WebServer::~WebServer() {
    delete _server;
    if (_measurements != NULL) delete _measurements;
}

void WebServer::addUriHandler(const char *uri, WebServer::THandlerFunction handler) {
    _server->on(uri, handler);
}

void WebServer::setup(const char* deviceName, Measurement** measurements, ConfigManager* configManager) {
    _deviceName = deviceName;
    _measurements = measurements;
    _configManager = configManager;
    addUriHandler("/", std::bind(&WebServer::handleRoot, this));
    addUriHandler("/configuration", std::bind(&WebServer::handleConfiguration, this));
    addUriHandler("/save", std::bind(&WebServer::handleSave, this));

    _server->begin();
    _logger.info(PSTR("Web server started"));
}

void WebServer::loop() {
    _server->handleClient();
}

void WebServer::handleRoot() {
    _logger.info(PSTR("WebServer handle root"));

    String html = FPSTR(RABBIT_HTTP_HEADER);
    html += FPSTR(RABBIT_HTTP_STYLE);
    html += FPSTR(RABBIT_HTTP_BODY);
    html.replace(F("{device}"), _deviceName);
    html += FPSTR(RABBIT_HTTP_TABLE);
    html.replace(F("{tableHeader}"), "Sensor data");

    int i = 0;
    while (_measurements[i]) {
        String tempHtml = FPSTR(RABBIT_HTTP_ITEM);
        tempHtml.replace(F("{key}"), _measurements[i]->getDescription());
        tempHtml.replace(F("{value}"), _measurements[i]->getStringValue());
        tempHtml.replace(F("{unit}"), _measurements[i]->getUnit());
        html += tempHtml;
        i++;
    }
    html += FPSTR(RABBIT_HTTP_TABLE_END);
    html += FPSTR(RABBIT_HTTP_CONFIG_BUTTON);
    html += FPSTR(RABBIT_HTTP_END);
    
    _server->send(200, F("text/html"), html); 
}

void WebServer::handleConfiguration() {
    _logger.info(PSTR("WebServer handle configuration"));

    String html = FPSTR(RABBIT_HTTP_HEADER);
    html += FPSTR(RABBIT_HTTP_STYLE);
    html += FPSTR(RABBIT_HTTP_BODY);
    html.replace(F("{device}"), _deviceName);
    html += FPSTR(RABBIT_HTTP_FORM);
    html += FPSTR(RABBIT_HTTP_TABLE);
    html.replace(F("{tableHeader}"), "Settings");

    ConfigParameter** parameters = _configManager->getParameters();
    int i = 0;
    while (parameters[i]) {
        String tempHtml = FPSTR(RABBIT_HTTP_CONFIG_ITEM);
        tempHtml.replace(F("{name}"), parameters[i]->getId());
        tempHtml.replace(F("{desc}"), parameters[i]->getDescription());
        tempHtml.replace(F("{value}"), parameters[i]->getValue());
        html += tempHtml;
        i++;
    }
    html += FPSTR(RABBIT_HTTP_TABLE_END);
    html += FPSTR(RABBIT_HTTP_BUTTON_ROW);
    html += FPSTR(RABBIT_HTTP_CANCEL_BUTTON);
    html += FPSTR(RABBIT_HTTP_SAVE_BUTTON);
    html += FPSTR(RABBIT_HTTP_FORM_END);
    html += FPSTR(RABBIT_HTTP_END);
    
    _server->send(200, F("text/html"), html); 
}


void WebServer::handleSave() {
    _logger.log(LOGLEVEL_INFO, PSTR("WebServer handle save - args = %u"), _server->args());

    String html = FPSTR(RABBIT_HTTP_HEADER);
    html += FPSTR(RABBIT_HTTP_STYLE);
    html += FPSTR(RABBIT_HTTP_BODY);
    html.replace(F("{device}"), _deviceName);

    ConfigParameter** parameters = _configManager->getParameters();
    bool save = false;
    int i = 0;
    while (parameters[i]) {
        String value = _server->arg(parameters[i]->getId());
        if (value != emptyString) {
            save = true;
            _logger.log(LOGLEVEL_INFO, PSTR("Set config parameter %s to %s"), parameters[i]->getId(), value.c_str());
            parameters[i]->setValue(value.c_str());
        }
        i++;
    }
    if (save) {
        _configManager->saveParameters();
        html += "Configuration saved!"; // TODO
    }

    html += FPSTR(RABBIT_HTTP_END);
    
    _server->send(200, F("text/html"), html); 
}
