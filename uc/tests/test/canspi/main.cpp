#include <WProgram.h>
#include "SPI.h"

const int button_pin = 2;
const int cs_pin = 10;

#include "mcp.h"

extern "C" 
void __cxa_pure_virtual(void) {

    for(;;) {
        // error handling code
    }
}

byte last_state;

void setup() {
    pinMode(button_pin, INPUT);
    last_state = digitalRead(button_pin);

    Serial.begin(9600);
    pinMode(cs_pin, OUTPUT);
    SPI.begin();
}

void worker() {
    mcp_probe();
    //mcp_probe_loopback();
}

/*
void handle_msg() {
    msg = mcp_get_msg();

    if(msg == TURN_ON_LIGTH) {
        turn_on_ligth():
        delay(1000);
        turn_off_ligth();
    }   
}
*/

void loop() {
    byte state = digitalRead(button_pin);

    if(last_state != state) {
        last_state = state;
        Serial.println(state, HEX);

        if(state == HIGH) worker();
    }

 //   if(mcp_check_msg()) handle_msg();

}

int main() {
    init();

    setup();
    for(;;)
        loop();

    return 0;
}

