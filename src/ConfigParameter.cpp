#include "ConfigParameter.h"
#include <Arduino.h>
#include <stdio.h>

#define INT_MAXLENGTH 11
#define FLOAT_MAXLENGTH 9

ConfigParameter::ConfigParameter(const char *id, const char *description, const char *defaultValue, int maxLength, const char *mqttTopic) {
  init(id, description, maxLength, mqttTopic);
  if (defaultValue != NULL) {
    setValue(defaultValue);
  }
}

ConfigParameter::ConfigParameter(const char *id, const char *description, long defaultValue, const char *mqttTopic) {
    init(id, description, INT_MAXLENGTH, mqttTopic);
    setValue(defaultValue);
}

ConfigParameter::ConfigParameter(const char *id, const char *description, float defaultValue, int precision, const char *mqttTopic) {
    init(id, description, FLOAT_MAXLENGTH, mqttTopic);
    setValue(defaultValue);
}	

void ConfigParameter::init(const char *id, const char *description, int maxLength, const char *mqttTopic) {
  _id = id;
  _description = description;
  _length = maxLength;
  _mqttTopic = mqttTopic;
  _value = new char[maxLength + 1];
  for (int i = 0; i < maxLength + 1; i++) {
    _value[i] = 0;
  }
}

ConfigParameter::~ConfigParameter() {
  if (_value != NULL)
    delete[] _value;
}

const char* ConfigParameter::getId() {
  return _id;
}

const char* ConfigParameter::getValue() {
  return _value;
}

long ConfigParameter::getIntValue() {
  return atol(_value);
}

float ConfigParameter::getFloatValue() {
  return atof(_value);
}

void ConfigParameter::setValue(const char *value) {
    strcpy(_value, value);
}

void ConfigParameter::setValue(long value) {
    itoa(value, _value, 10);
}	

void ConfigParameter::setValue(float value) {
    sprintf(_value, "%.*f", _precision, value);
}