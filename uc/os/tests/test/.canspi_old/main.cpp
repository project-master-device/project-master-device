#include "WProgram.h"
#include "SPI.h"

#define SCK 13
#define SO 12
#define SI 11
#define SS 10  

//opcodes
#define RESET 0xC0
#define READ  0x03
#define WRITE 0x02

extern "C" 
void __cxa_pure_virtual(void) {

    for(;;) {
        // error handling code
    }
}

//byte clr;
byte STATUS;

void setup() {
    Serial.println("Initialization started");
    Serial.begin(9600);

    pinMode(SCK,OUTPUT);
    pinMode(SO,OUTPUT);
    pinMode(SI, INPUT);
    pinMode(SS, OUTPUT);
    pinMode(RESET,OUTPUT);
    
    SPI.transfer(RESET);
    Serial.println("Initialization done");
}

void loop() {
    delay(10);
    digitalWrite(SS, LOW);
    SPI.transfer(READ);
    SPI.transfer(0x3E);// CANSTAT address of TXB0
    STATUS = SPI.transfer(0xFF);
    digitalWrite(SS, HIGH);
    delay(10);
    Serial.println(STATUS, HEX);
    delay(1000);
}

int main() {
    init();

    setup();
    
    for(;;) {
        loop();
    }

    return 0;
}

