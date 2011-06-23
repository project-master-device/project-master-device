#include <util/delay.h>
#include <avr/io.h>

#include "mcp.h"

#if defined (__AVR_ATmega168__)
#define LPCK_PORT PORTD
#define LPCK_DDR DDRD
#define LPCK PD2
#define BUTTON_PIN PIND
#define BUTTON_DDR DDRD
#define BUTTON PD3

#elif defined (__AVR_ATmega162__)
// Relay
//#define LPCK_PORT PORTA
//#define LPCK_DDR DDRA
//#define LPCK PA1
#define LPCK_PORT PORTC
#define LPCK_DDR DDRC
#define LPCK PC3
#define BUTTON_PIN PINC
#define BUTTON_DDR DDRC
#define BUTTON PC2

FUSES = { 
    .low = (FUSE_CKOUT),
    .high = (HFUSE_DEFAULT & FUSE_BOOTRST),
    .extended = (EFUSE_DEFAULT & FUSE_BODLEVEL0),
};

#else
#warning "device type not defined"
#endif

void lampochka_init(void) {
    LPCK_DDR |= (1 << LPCK);
}

void button_init(void) {
    BUTTON_DDR &= ~(1 << BUTTON);
}

void lampochka(uint8_t n) {
    while(n) {
        LPCK_PORT |= (1 << LPCK);
        _delay_ms(200);
        LPCK_PORT &= ~(1 << LPCK);
        _delay_ms(200);
        n--;
    }
}

uint8_t get_button_state() {
    return BUTTON_PIN & (1 << BUTTON);
}

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

void main(void) {
    uint8_t r;
    uint8_t zzz = 0;

#ifdef __AVR_ATmega162__
    TCCR2 |= (1 << CS20);
    TCCR2 |= (1 << FOC2);
    TCCR2 |= (1 << WGM21);
    TCCR2 |= (1 << COM20);
    
//    TCCR2 |= (1 << COM21);
//    TCCR2 |= (1 << WGM20);
    
    DDRB |= (1 << PB1);
    _delay_ms(1000);
#endif

    lampochka_init();
    //for(;;) {lampochka(1);}

    button_init();
    spi_init();
    mcp_init();

#if 1
    mcp_reset();

    if(mcp_config_rate(MCP_RATE_125KBPS)) {
        lampochka(1);
    } else {
        for(;;) {
            lampochka(3);
            _delay_ms(1000);
        }
    }

    if(mcp_set_normal_mode()) {
        lampochka(1);
        mcp_modify_reg(MCP_CANINTE, MCP_CANINTE_TX0IE, 0);
        mcp_modify_reg(MCP_CANINTE, MCP_CANINTE_TX1IE, 0);
        mcp_modify_reg(MCP_CANINTE, MCP_CANINTE_TX2IE, 0);
    } else {
        for(;;) {
            lampochka(2);
            _delay_ms(1000);
        }
    }

    //mcp_modify_reg(MCP_RXB0CTRL, MCP_RXBCTRL_RXM_MASK, MCP_RXBCTRL_RXM_MASK);

#endif

    for (;;) {
#if 1
        uint8_t stat = mcp_read_status();
        can_frame msg;
       
        if(stat & MCP_STAT_RX0IF) {
           mcp_read_frame(MCP_RXB0CTRL + 1, &msg);
           mcp_modify_reg(MCP_CANINTF, MCP_CANINTF_RX0IF, 0);

//           lampochka(msg.can_id + 1);
           lampochka(msg.data[0] + 1);
        } else if(stat & MCP_STAT_RX1IF) {
           mcp_read_frame(MCP_RXB1CTRL + 1, &msg);
           mcp_modify_reg(MCP_CANINTF, MCP_CANINTF_RX1IF, 0);

//           lampochka(msg.can_id + 1);
           lampochka(msg.data[0] + 1);
        }
#endif
#if 1
        if(get_button_state()) {
            uint8_t buf_ctrl = MCP_TXB0CTRL;
            if(mcp_get_free_txbuf(&buf_ctrl)) {
                can_frame msg;

                msg.can_id = 7;
                msg.can_dlc = 1;
                msg.can_rtr = 0;

                if(zzz > 8) zzz = 0;

                for(r = 0; r < 8; ++r)
                    msg.data[r] = 0;

                msg.data[0] = 2;//zzz++;

                mcp_write_frame(buf_ctrl + 1, &msg);
                mcp_start_transmit(buf_ctrl);

#if 1
                //if(mcp_check_reg(buf_ctrl, MCP_TXB_TXREQ_MASK, MCP_TXB_TXREQ_PENDING)) {
                    lampochka(1);
                //}
#else
                while(mcp_check_reg(MCP_TXB0CTRL, MCP_TXB_TXERR, MCP_TXB_TXERR)) {
                    lampochka(1);
                }

                for (;;) {
                    uint8_t ctrl = mcp_read_reg(buf_ctrl);

                    if ( (ctrl & MCP_TXB_TXREQ_MASK) == MCP_TXB_TXREQ_NOT_PENDING ) {
                        break;
                    }
                    lampochka(3);
                    _delay_ms(1000);
                }
#endif
            }
            //_delay_ms(1000);
        }
#endif
    }
}

