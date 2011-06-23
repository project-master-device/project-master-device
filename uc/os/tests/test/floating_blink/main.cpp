#include "Led.h"

extern "C" 
void __cxa_pure_virtual(void) {

    for(;;) {
        // error handling code
    }
}

// trueclass to avoid of using global variables
class Main {
private:

    Led led;

    unsigned long last_time;
    unsigned long interval;
    unsigned long st;

    static Main _instance;

protected:
   
    Main() {
        setup();
    }

    void update() {
        unsigned long time = millis();

        if(time - last_time > interval) {
            last_time = time;
            led.toggle();
    
            interval -= st;

            if(interval == st) {
                st = -st;
            } else if(interval == 1000) {
                st = -st;
            }
        
            Serial.println("Current interval:");
            Serial.println(interval, DEC);
            Serial.println();
        }
    }

public:
    
    void setup() {
        last_time = 0;
        interval = 1000;
        st = 20;
        Serial.begin(9600);
    }
    
    void loop() {
        // do something

        // toggle led and change interval, if the time has come
        update();
    }

    static Main& instance() {
        return _instance;
    }
};

Main Main::_instance;

int main() {
    init();
    
    for(;;)
        Main::instance().loop();

    return 0;
}

