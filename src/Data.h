#ifndef Data_h
#define Data_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Arduino.h>

/**
 * Abstract class for representing sensor data.
 */
class BaseData {
    public:
        enum DataType {
            None,
            Bool,
            Int,
            Float
        };

        /**
         * Call this constructor from subclasses with the correct data type.
         */
        BaseData(DataType dataType = None) { _dataType = dataType; }

        /**
         * Copy constructor. Call this from the specific copy constructor in a subclass.
         */
        BaseData(const BaseData& other) {
            _dataType = other._dataType;
        }

        /**
         * Copy method. Override this in subclasses and call the specific copy constructor of the subclass.
         */
        virtual BaseData* copy() const {
            return new BaseData(*this);
        }

        /**
         * Gets the value as a string. Override this in subclasses.
         */
        virtual const char* stringValue() const { return ""; };

        /**
         * Returns true when both the data type and value are the same. Override this in subclasses.
         */
        virtual bool equals(const BaseData& other) const {
            return other.dataType() == None;
        }

        inline const DataType dataType() const { return _dataType; }

    private:
        DataType _dataType;
};

/**
 * Data class for boolean data: ON/OFF, HIGH/LOW, OPEN/CLOSE, etc.
 */
class BoolData : public BaseData {
    public:
        BoolData(bool data) : 
            BaseData(Bool) {
            _value = data;
            _buf[0] = _value ? '1' : '0';
            _buf[1] = 0;
        }

        BoolData(const BoolData& other) :
            BaseData(other) {
            _value = other._value;
            _buf[0] = other._buf[0];
            _buf[1] = 0;
        }

        virtual BaseData* copy() const {
            return new BoolData(*this);
        }

        bool value() const { return _value; }

        virtual const char* stringValue() const { return _buf; }

        virtual bool equals(const BaseData& other) const {
            return other.dataType() == dataType() && ((BoolData&)other)._value == _value;
        }
    
    private:
        bool _value;
        char _buf[2];
};

/**
 * Data class for integral number data.
 */
class IntegerData : public BaseData {
    public:
        IntegerData(long data)  : 
            BaseData(Int) {
            _value = data;
            itoa(_value, _buf, 10);
        }

        IntegerData(const IntegerData& other) :
            BaseData(other) {
            _value = other._value;
            strcpy(_buf, other._buf);
        }

        virtual BaseData* copy() const {
            return new IntegerData(*this);
        }

        int value() const { return _value; }

        virtual const char* stringValue() const { return _buf; }

        virtual bool equals(const BaseData& other) const {
            return other.dataType() == dataType() && ((IntegerData&)other)._value == _value;
        }
    
    private:
        long _value;
        char _buf[12]; // 10 digits, sign and \0
};

/**
 * Data class for floating point number data.
 * The precision parameter determines the number of relevant decimals. It is used for:
 * - determining whether 2 values are the same. E.g. when set to 2, differences smaller than 0.01 are ignored.
 * - formatting the data as a string. E.g. when set to 2, a value of 3.6666 is formatted as 3.67
 */
class FloatData : public BaseData {
    public:
        FloatData(float data = 0, int precision = 2) : 
            BaseData(Float) {
            _value = data;
            _precision = precision;
            _smallestDifference = pow(0.1, precision);
            sprintf(_buf, "%.*f", _precision, _value);
        }

        FloatData(const FloatData& other) :
            BaseData(other) {
            _value = other._value;
            _smallestDifference = other._smallestDifference;
            _precision = other._precision;
            strcpy(_buf, other._buf);
        }

        virtual BaseData* copy() const {
            return new FloatData(*this);
        }

        float value() const { return _value; }

        virtual const char* stringValue() const { return _buf; }

        virtual bool equals(const BaseData& other) const {
            return other.dataType() == dataType() && fabs(((FloatData&)other)._value - _value) < _smallestDifference;
        }
    
    private:
        float _value;
        float _smallestDifference;
        int _precision;
        char _buf[10]; // not sure, should be enough
};

#endif
