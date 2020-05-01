#ifndef Data_h
#define Data_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Arduino.h>

class BaseData {
    public:    
        enum DataType {
            None,
            Bool,
            Int,
            Float
        };

        BaseData(DataType dataType = None) { _dataType = dataType; }

        BaseData(const BaseData& other) {
            _dataType = other._dataType;
        }

        virtual BaseData* copy() const {
            return new BaseData(*this);
        }

        virtual const char* stringValue() const { return ""; };

        virtual bool equals(const BaseData& other) const {
            return other.dataType() == None;
        }

        inline const DataType dataType() const { return _dataType; }

    private:
        DataType _dataType;
};

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
