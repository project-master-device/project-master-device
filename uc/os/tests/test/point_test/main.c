#include <util/delay.h>
#include <avr/io.h>

#include "mcp.h"

#define MAN_NODE_CAN_ID     1
#define MAN_NODE_ALLOW      1
#define MAN_NODE_DISALLOW   0

#define MAN_OPEN_REQUEST_CODE 123

#if defined (__AVR_ATmega168__)
#   define MAN_CONTROLLER_HW_ADDR   323
#   define MAN_CONTROLLER_DEVICE_ID 1

#   define LED_OK_PORT PORTD
#   define LED_OK_DDR DDRD
#   define LED_OK PD2

#   define LED_FAIL_PORT PORTD
#   define LED_FAIL_DDR DDRD
#   define LED_FAIL PD4

#   define BUTTON_PIN PIND
#   define BUTTON_DDR DDRD
#   define BUTTON PD3

#elif defined (__AVR_ATmega162__)
#   define MAN_CONTROLLER_HW_ADDR   5
#   define MAN_CONTROLLER_DEVICE_ID 1

#   define LED_OK_PORT PORTA
#   define LED_OK_DDR DDRA
#   define LED_OK PA1

#   define LED_FAIL_PORT PORTA
#   define LED_FAIL_DDR DDRA
#   define LED_FAIL PA2

#   define BUTTON_PIN PINC
#   define BUTTON_DDR DDRC
#   define BUTTON PC2

FUSES = { 
    .low = (FUSE_CKOUT),
    .high = (HFUSE_DEFAULT & FUSE_BOOTRST),
    .extended = (EFUSE_DEFAULT & FUSE_BODLEVEL0),
};

#else
#   warning "device type not defined"

#endif

void blink(uint8_t port, uint8_t pin, uint8_t num) {
    while(num) {
        port |= (1 << pin);
        _delay_ms(200);
        port &= ~(1 << pin);
        _delay_ms(200);
        num--;
    }
}

void led_init(void) {
    LED_OK_DDR |= (1 << LED_OK);
    LED_FAIL_DDR |= (1 << LED_FAIL);
}

void led_ok_on(void) {
    LED_OK_PORT |= (1 << LED_OK);
}

void led_ok_off(void) {
    LED_OK_PORT &= ~(1 << LED_OK);
}

void led_ok_blink(uint8_t num) {
    while(num) {
        LED_OK_PORT |= (1 << LED_OK);
        _delay_ms(200);
        LED_OK_PORT &= ~(1 << LED_OK);
        _delay_ms(200);
        num--;
    }
}

void led_fail_on(void) {
    LED_FAIL_PORT |= (1 << LED_FAIL);
}

void led_fail_off(void) {
    LED_FAIL_PORT &= ~(1 << LED_FAIL);
}

void led_fail_blink(uint8_t num) {
    while(num) {
        LED_FAIL_PORT |= (1 << LED_FAIL);
        _delay_ms(200);
        LED_FAIL_PORT &= ~(1 << LED_FAIL);
        _delay_ms(200);
        num--;
    }
}


void button_init(void) {
    BUTTON_DDR &= ~(1 << BUTTON);
}

uint8_t get_button_state(void) {
    return BUTTON_PIN & (1 << BUTTON);
}
/*
/////
uint8_t can_check_error(void) {
    uint8_t eflg = mcp_read_reg(MCP_EFLG);

    if ( eflg & MCP_EFLG_ERRORMASK ) {
        return 0;
    }  else {
        return 1;
    }
}
//////
*/
void init_all(void) {

#ifdef __AVR_ATmega162__
    TCCR2 |= (1 << CS20);
    TCCR2 |= (1 << FOC2);
    TCCR2 |= (1 << WGM21);
    TCCR2 |= (1 << COM20);
    //TCCR2 |= (1 << COM21);
    //TCCR2 |= (1 << WGM20);
    DDRB |= (1 << PB1);
    _delay_ms(1000);
#endif

    led_init();
    button_init();
    spi_init();
    mcp_init();

    mcp_reset();

    if(mcp_config_rate(MCP_RATE_125KBPS)) {
        led_ok_blink(1);
    } else {
        for(;;) {
            led_fail_blink(2);
            _delay_ms(1000);
        }
    }

    if(mcp_set_normal_mode()) {
        led_ok_blink(1);
        mcp_modify_reg(MCP_CANINTE, MCP_CANINTE_TX0IE, 0);
        mcp_modify_reg(MCP_CANINTE, MCP_CANINTE_TX1IE, 0);
        mcp_modify_reg(MCP_CANINTE, MCP_CANINTE_TX2IE, 0);
    } else {
        for(;;) {
            led_fail_blink(4);
            _delay_ms(1000);
        }
    }

    //mcp_modify_reg(MCP_RXB0CTRL, MCP_RXBCTRL_RXM_MASK, MCP_RXBCTRL_RXM_MASK);
}

void handle_msg(const can_frame * msg) {
    if(msg->data[3] == 1) {
        led_ok_on();
        _delay_ms(5000);
        led_ok_off();
    } else if (msg->data[3] == 0) {
        led_fail_on();
        _delay_ms(5000);
        led_fail_off();
    }
}

void read_can(void) {
    uint8_t stat;
    can_frame msg;
 
    stat = mcp_read_status();
       
    if(stat & MCP_STAT_RX0IF) {
        mcp_read_frame(MCP_RXB0CTRL + 1, &msg);
        mcp_modify_reg(MCP_CANINTF, MCP_CANINTF_RX0IF, 0);

//        if(msg.can_id == MAN_CONTROLLER_HW_ADDR)
            handle_msg(&msg);

    } else if(stat & MCP_STAT_RX1IF) {
        mcp_read_frame(MCP_RXB1CTRL + 1, &msg);
        mcp_modify_reg(MCP_CANINTF, MCP_CANINTF_RX1IF, 0);

 //       if(msg.can_id == MAN_CONTROLLER_HW_ADDR)
            handle_msg(&msg);
    }
}

void write_can(void) {
    uint8_t num, buf_ctrl;
    can_frame msg;

    if(mcp_get_free_txbuf(&buf_ctrl)) {
        msg.can_id = 0;
        msg.can_dlc = 8;
        msg.can_rtr = 0;

        for(num = 0; num < 8; ++num)
            msg.data[num] = 0;

        // protocol version 1
        msg.data[7] = 1;

        // pack controller hw_arrr from MAN_CONTROLLER_HW_ADDR (32 bit)
        msg.data[7] |= (uint8_t)((MAN_CONTROLLER_HW_ADDR & 0x00000001) << 8);
        msg.data[6]  = (uint8_t)((MAN_CONTROLLER_HW_ADDR & 0x000001FE) >> 1);
        msg.data[5]  = (uint8_t)((MAN_CONTROLLER_HW_ADDR & 0x0001FE00) >> 9);
        msg.data[4] |= (uint8_t)((MAN_CONTROLLER_HW_ADDR & 0x000E0000) >> 17);

        // pack device_id
        msg.data[4] |= (uint8_t)(MAN_CONTROLLER_DEVICE_ID & 0x78 << 3);

        // set last frame bit
        msg.data[4] |= (0x01 << 7);

        // write key
        msg.data[3] = MAN_OPEN_REQUEST_CODE;

        mcp_write_frame(buf_ctrl + 1, &msg);
        mcp_start_transmit(buf_ctrl);

        //if(mcp_check_reg(buf_ctrl, MCP_TXB_TXREQ_MASK, MCP_TXB_TXREQ_PENDING)) {
        led_ok_blink(1);
        //}
    }
}

void main(void) {
    init_all();

    for (;;) {
        read_can();

        if(get_button_state())
            write_can();

        //_delay_ms(10);
    }
}

