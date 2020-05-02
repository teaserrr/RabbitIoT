#include "WebServer.h"
#include "HtmlDefines.h"

WebServer* WebServer::_instance;

void serverHandleRoot() {
    WebServer::_instance->handleRoot();
}

WebServer::WebServer(const Logger& logger, int port) {
    _logger = logger;
    _server = new ESP8266WebServer(port);
    _measurements = NULL;
    _instance = this;
}

WebServer::~WebServer() {
    delete _server;
    if (_measurements != NULL) delete _measurements;
}

void WebServer::addUriHandler(const char *uri, WebServer::THandlerFunction handler) {
    _server->on(uri, handler);
}

void WebServer::setup(const char* deviceName, Measurement** measurements) {
    _deviceName = deviceName;
    _measurements = measurements;
    addUriHandler("/", serverHandleRoot);

    _server->begin();
    _logger.info("Web server started");
}

void WebServer::loop() {
    _server->handleClient();
}

void WebServer::handleRoot() {
    _logger.info("WebServer handle root");

    String html = FPSTR(RABBIT_HTTP_HEADER);
    html += FPSTR(RABBIT_HTTP_STYLE);
    html += FPSTR(RABBIT_HTTP_BODY);
    html.replace("{device}", _deviceName);
    html += FPSTR(RABBIT_HTTP_TABLE);

    int i = 0;
    while (_measurements[i]) {
        String tempHtml = FPSTR(RABBIT_HTTP_ITEM);
        tempHtml.replace("{key}", _measurements[i]->getDescription());
        tempHtml.replace("{value}", _measurements[i]->getStringValue());
        tempHtml.replace("{unit}", _measurements[i]->getUnit());
        html += tempHtml;
        i++;
    }
    html += FPSTR(RABBIT_HTTP_TABLE_END);
    html += FPSTR(RABBIT_HTTP_LINK_BUTTON);
    html += FPSTR(RABBIT_HTTP_END);
    
    _server->send(200, "text/html", html); 
}
