#define __AVR_ATmega162__
//#define __AVR_ATmega8__

#include <avr/common.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "spi.h"
#include "can.h"
#include "delay.h"
#include "timebase.h"
#include "mcp2515.h"

#include "wiring.h"

#define RECIEVE_DATA_BYTE       0
#define RECIEVE_DATA_BYTE_VALUE 3

#define SEND_DATA_BYTE          0
#define SEND_DATA_BYTE_VALUE    3

#define SEND_ERR_MAX            1
#define ERR_SIG_NUMBER          4
#define SEND_ERR_SIG_NUMBER     10
#define RECIEVE_OK_SIG_NUMBER   12
#define RECIEVE_FAIL_SIG_NUMBER 2

unsigned int send_errors = 0;

static void turn_relay() {
    PORTA ^= (1 << PA1);
}

static int get_button_state() {
    int res = 0;

    uint8_t pc = PINC & (1 << PC2);
    if(pc == (1 << PC2)) {
        // Button pressed
       res = 1; 
    }

    return res;
}

static void process_can_msg() {
    CanMessage msg;
    int i = 0;

    if(can_checkReceive() == CAN_MSGAVAIL) {
        uint8_t res = can_readMessage(&msg);

        if(res == CAN_OK) {
            if(msg.dta[RECIEVE_DATA_BYTE] == RECIEVE_DATA_BYTE_VALUE) {

                for(i = 0; i < RECIEVE_OK_SIG_NUMBER; ++i) {
                    _delay_ms(100);
                    //blink_ok_led();
                }
                //off_ok_led();

            } else {
            }

        } else {
        }
    } else {
        for(i = 0; i < RECIEVE_FAIL_SIG_NUMBER; ++i) {
            _delay_ms(100);
            //blink_err_led();
        }
        //off_err_led();
    }
}

static void send_can_msg() {
    int i = 0;
    int j = 0;

    CanMessage msg;
    can_initMessageStruct(&msg);

    msg.identifier = CANDEFAULTIDENT;

    for(; i < CAN_MAX_CHAR_IN_MESSAGE; ++i) {
        msg.dta[i] = 0;
    }
    
    msg.dta[SEND_DATA_BYTE] = SEND_DATA_BYTE_VALUE;

    if(can_sendMessage(&msg) != CAN_OK) {
        send_errors += 1;

        if(send_errors == SEND_ERR_MAX) {
            // Bad news
            for(j = 0; j < SEND_ERR_SIG_NUMBER; ++j) {
                turn_relay();
                _delay_ms(100);
            }
            send_errors = 0;
        }
    }

    if(can_checkError() != CAN_OK) {
        for(j = 0; j < ERR_SIG_NUMBER; ++j) {
            turn_relay();
            _delay_ms(500);
        }
    }
}

static void setup_can() {
    uint8_t res = can_init(CAN_125KBPS);

	if (res != CAN_OK) {
        for(;;) {
            _delay_ms(3000);
            turn_relay();
        }
    }

    res = mcp2515_setCANCTRL_Mode(MODE_NORMAL);
	if (res != MCP2515_OK) {
        for(;;) {
            _delay_ms(1000);
            turn_relay();
        }
    }
}

static void setup_interrupts() {
    /*
    // Setup Timer 0 
    TCCR0A = 0b00000000;   // Normal Mode 
    TCCR0B = 0b00000010;   // Div8 Prescaler 
    TCNT0 = 0;             // Initial value 
    */
    TIMSK = (1 << TOV0);
    TCNT0 = 0;
    TCCR0 = (1 << CS00);
    /*TCCR0 &= ~(1 << CS00);
    TCCR0 |=  (1 << CS01);
    TCCR0 &= ~(1 << CS02);
    */
    // Enable interrupts as needed 
    sei();                 // Global Interrupts 
}

/*ISR(TIMER0_OVF_vect) {
    PORTB ^= (1 << PB7);
    TCNT0 = 0xFF;
}*/

static void setup_all() {
    //init();

    // Set relay pin output mode
    //DDRA  |=  (1 << PA1);
    //DDRB  |=  (1 << PB7);
    //PORTA &= ~(1 << PA1);

    // Set "button" input pin
    //DDRC  &= ~(1 << PC2);

    //timebase_init();
    spi_init();
//    setup_can();
}

//#define SELECT_CODE
//#define SEND_CODE

int main() {

    setup_all();

    for(;;) {
        _delay_ms(500);
#ifdef SEND_CODE
        _delay_ms(500);

        if(get_button_state()) {
            turn_relay();
            send_can_msg();
        }
#endif
#ifdef SELECT_CODE
        MCP2515_SELECT();
        turn_relay();
        _delay_ms(300);
        MCP2515_UNSELECT();
        turn_relay();
        _delay_ms(300);
#endif
    }

    return 0;
}

