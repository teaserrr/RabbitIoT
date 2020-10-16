#ifndef StateListener_h
#define StateListener_h

#define MAX_STATELISTENERS 5

class Measurement;

//typedef void (*ListenerFunc) (const Measurement* measurement);
typedef std::function<void(const Measurement* measurement)> ListenerFunc;

class StateListener {
    public: 
        StateListener(ListenerFunc listenerFunc = NULL) {
            _listenerFunc = listenerFunc;
        }

        virtual void onStateChanged(Measurement* measurement) { 
            if (_listenerFunc)
                _listenerFunc(measurement);
        }

    private:
        ListenerFunc _listenerFunc;
};

#endif
