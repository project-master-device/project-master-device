#define __AVR_ATmega168__
//#define __AVR_ATmega8__

#include <avr/common.h>
#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"
#include "can.h"
#include "delay.h"
#include "timebase.h"
#include "mcp2515.h"

#include "wiring.h"

#define SEND_DATA_BYTE          0
#define SEND_DATA_BYTE_VALUE    3

#define RECIEVE_DATA_BYTE       0
#define RECIEVE_DATA_BYTE_VALUE 3

#define SEND_ERR_MAX            1
#define SEND_ERR_SIG_NUMBER     10

#define RECIEVE_OK_SIG_NUMBER   12
#define RECIEVE_FAIL_SIG_NUMBER 2

#define ERR_SIG_NUMBER          4

unsigned int send_errors = 0;

static void blink_err_led() {
    PORTD ^= (1 << PD2);
}

static void off_err_led() {
    PORTD &= ~(1 << PD2);
}

static void on_err_led() {
    PORTD |= (1 << PD2);
}

static void blink_ok_led() {
    PORTD ^= (1 << PD4);
}

static void off_ok_led() {
    PORTD &= ~(1 << PD4);
}

static void on_ok_led() {
    PORTD |= (1 << PD4);
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
                    blink_ok_led();
                }
                off_ok_led();

            } else {
            }

        } else {
        }
    } else {
        for(i = 0; i < RECIEVE_FAIL_SIG_NUMBER; ++i) {
            _delay_ms(100);
            blink_err_led();
        }
        off_err_led();
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
                blink_err_led();
                _delay_ms(100);
            }
            send_errors = 0;
        }
    }

    if(can_checkError() != CAN_OK) {
        for(j = 0; j < ERR_SIG_NUMBER; ++j) {
            //turn_relay();
            blink_err_led();
            _delay_ms(500);
        }
    }
}

static void setup_can() {

    uint8_t res = can_init(CAN_125KBPS);

	if (res != CAN_OK) {
        for(;;) {
            _delay_ms(3000);
            blink_err_led();
        }
    }

    res = mcp2515_setCANCTRL_Mode(MODE_NORMAL);
	if (res != MCP2515_OK) {
        for(;;) {
            _delay_ms(1000);
            blink_err_led();
        }
    }
}

static void setup_all() {
    //init();

    // Set led pin's output mode
    DDRD  |=  ((1 << PD2) | (1 << PD4));
    off_err_led();
    off_ok_led();
 
////timebase_init();

    spi_init();
    setup_can();
}

int main() {

    setup_all();

    for(;;) {
        blink_ok_led();
        _delay_ms(500);
        send_can_msg();
        //process_can_msg();
/*
	    MCP2515_SELECT();
        _delay_ms(300);
	    MCP2515_UNSELECT();
*/
    }

    return 0;
}

