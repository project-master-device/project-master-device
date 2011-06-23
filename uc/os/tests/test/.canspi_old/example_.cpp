#include "WProgram.h"
#include "SPI.h"

#define SCK 13
#define SO 12
#define SI 11
#define SS 10  

//opcodes
#define RESET 2
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
    Serial.begin(9600);

    pinMode(SCK,OUTPUT);
    pinMode(SO,OUTPUT);
    pinMode(SI, INPUT);
    pinMode(SS, OUTPUT);
    pinMode(RESET,OUTPUT);

    /* 
        SPI interrupt disabled, SPI enabled, sends MSB first, MC is master,  
        clock polarity=0, clock phase=0(hence mode 0), SPI speed = 4MHz(fastest)
     */
    SPCR = ( (1<<SPE)|(1<<MSTR)); // 0101 0000
    //clr = SPSR;                                
    //clr = SPDR;

    /* 
        configure CAN by setting bit timing ( and hence baud rate at 1Mbps)
        1Bit=1 micro second.
        Let baud rate prescaler = 0
        Then TQ= 100ns
        i.e 1 bit = 10 TQ
        //sync seg= 1TQ, prog seg=2 TQ, PS1=3 TQ, PS2= 4TQ//
        also sampling is happening at 60% of the bit time.
    */

    digitalWrite(RESET,LOW); /* RESET CAN CONTROLLER*/
    delay(10);
    digitalWrite(RESET,HIGH);
    delay(10);

    digitalWrite(SS,LOW);
    Spi.transfer(WRITE);
    Spi.transfer(0x3F);// address of CANCTRL register of TXB0
    Spi.transfer(0x90);// select Configuration mode for programming bit-time
    digitalWrite(SS,HIGH);
    delay(10);

    digitalWrite(SS,LOW);
    Spi.transfer(WRITE);
    Spi.transfer(0x2A);// address of CNF1 register
    Spi.transfer(0x80);//1000 0000- SJW= 3TQ, BRP=0
    digitalWrite(SS,HIGH);
    delay(10);

    digitalWrite(SS,LOW);
    Spi.transfer(WRITE);
    Spi.transfer(0x29);// address of CNF2 register
    Spi.transfer(0x91);// 1001 0001- sampling at sample point,prog seg= 2TQ, PS1= 3TQ
    digitalWrite(SS,HIGH);
    delay(10);

    digitalWrite(SS,LOW);
    Spi.transfer(WRITE);
    Spi.transfer(0x28);// address of CNF3 register
    Spi.transfer(0x03);// PS2= 4TQ
    digitalWrite(SS,HIGH);
    delay(10);

    // initialize CAN
    delay(10);
    digitalWrite(SS,LOW);
    Spi.transfer(WRITE);// write instruction
    Spi.transfer(0x3F);// address of CANCTRL register of TXB0
    Spi.transfer(0x50);// select loop back mode for testing
    digitalWrite(SS,HIGH);
    delay(10);

    digitalWrite(SS,LOW);
    Spi.transfer(WRITE);// write instruction
    Spi.transfer(0x6F);// address of CANCTRL register of RXB0
    Spi.transfer(0x50);// select loop back mode for testing 01010000
    digitalWrite(SS,HIGH);
    delay(10);

    SPI.begin();
}

int main() {
    init();

    setup();
    
    for(;;) {
        //todo
    }

    return 0;
}

