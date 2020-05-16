#ifndef ConfigParameter_h
#define ConfigParameter_h

/**
 * Represents a configuration parameter.
 */
class ConfigParameter {
  public:
	ConfigParameter(const char *id, const char *description, const char *defaultValue, int maxLength, const char *mqttTopic);
	ConfigParameter(const char *id, const char *description, long defaultValue, const char *mqttTopic);
	ConfigParameter(const char *id, const char *description, float defaultValue, int precision, const char *mqttTopic);
	~ConfigParameter();

	const char *getId();
	const char* getDescription() const { return _description; }
	const char* getMqttTopic() const { return _mqttTopic; }
	int getMaxLength() const { return _length; }

	const char *getValue();
	long getIntValue();
	float getFloatValue();
	
	void setValue(const char *value);
	void setValue(long value);
	void setValue(float value);
	
  private:
	const char *_id;
	const char *_description;
	char       *_value;
	int         _length;
    int         _precision; // used by floats only
	const char *_mqttTopic;
	
	void init(const char *id, const char *description, int maxLength, const char *mqttPath);
};

#endif
