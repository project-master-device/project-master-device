#include "WProgram.h"

class Led {
private:
    
    int pin_;
    int state_;
  
public:
    
    Led(int state = LOW, int pin = 13) {
        pin_ = pin;   
        pinMode(pin_, OUTPUT);
        state_ = state;
    }
  
    void on() {
        set_state(HIGH);
    }
  
    void off() {
        set_state(LOW);
    }

    void toggle() {
        if(state_ == LOW) {
            set_state(HIGH);
        } else {
            set_state(LOW);
        }
    }

private:

    void set_state(int state) {
        state_ = state;
        digitalWrite(pin_, state);
    }
};

