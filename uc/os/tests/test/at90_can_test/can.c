#include "can.h"

#include <avr/io.h>
#include <util/delay.h>

// Macro {
#define CAN_SET_STD_ID_10_4(identifier)  (((*((uint8_t *)(&(identifier))+1))<<5)+((* (uint8_t *)(&(identifier)))>>3))
#define CAN_SET_STD_ID_3_0( identifier)  (( * (uint8_t *)(&(identifier))   )<<5)
#define CAN_SET_STD_ID(identifier)  { CANIDT1   = CAN_SET_STD_ID_10_4(identifier); \
                                      CANIDT2   = CAN_SET_STD_ID_3_0( identifier); \
                                      CANCDMOB &= (~(1 << IDE))                    ; }

#define CAN_GET_STD_ID(identifier)  { *((uint8_t *)(&(identifier))+1) =  CANIDT1>>5              ; \
                                      *((uint8_t *)(&(identifier))  ) = (CANIDT2>>5)+(CANIDT1<<3); }

#define CAN_SET_EXT_ID_28_21(identifier)  (((*((uint8_t *)(&(identifier))+3))<<3)+((*((uint8_t *)(&(identifier))+2))>>5))
#define CAN_SET_EXT_ID_20_13(identifier)  (((*((uint8_t *)(&(identifier))+2))<<3)+((*((uint8_t *)(&(identifier))+1))>>5))
#define CAN_SET_EXT_ID_12_5( identifier)  (((*((uint8_t *)(&(identifier))+1))<<3)+((* (uint8_t *)(&(identifier))   )>>5))
#define CAN_SET_EXT_ID_4_0(  identifier)   ((* (uint8_t *)(&(identifier))   )<<3)
#define CAN_SET_EXT_ID(identifier)  { CANIDT1   = CAN_SET_EXT_ID_28_21(identifier); \
                                      CANIDT2   = CAN_SET_EXT_ID_20_13(identifier); \
                                      CANIDT3   = CAN_SET_EXT_ID_12_5( identifier); \
                                      CANIDT4   = CAN_SET_EXT_ID_4_0(  identifier); \
                                      CANCDMOB |= (1 << IDE);                         }

#define CAN_GET_EXT_ID(identifier)  { *((uint8_t *)(&(identifier))+3) =  CANIDT1>>3              ; \
                                      *((uint8_t *)(&(identifier))+2) = (CANIDT2>>3)+(CANIDT1<<5); \
                                      *((uint8_t *)(&(identifier))+1) = (CANIDT3>>3)+(CANIDT2<<5); \
                                      *((uint8_t *)(&(identifier))  ) = (CANIDT4>>3)+(CANIDT3<<5); }

#define CAN_SET_RTR()     ( CANIDT4  |= (1<<RTRTAG)  )
#define CAN_CLEAR_RTR()   ( CANIDT4  &= ~(1<<RTRTAG) )

#define CAN_DLC_MASK    0x0F
#define DLC             DLC0
#define DLC_MSK         ((1<<DLC3)|(1<<DLC2)|(1<<DLC1)|(1<<DLC0))
#define CAN_GET_DLC()   ((CANCDMOB &  DLC_MSK) >> DLC)
#define CAN_GET_IDE()   ((CANCDMOB &  (1 << IDE)) >> IDE)
#define CAN_GET_RTR()   ((CANIDT4  &  (1 << RTRTAG)) >> RTRTAG)
#define CAN_CLEAR_MOB()        { uint8_t  volatile *__i_; for (__i_=&CANSTMOB; __i_<&CANSTML; __i_++) { *__i_=0x00 ;}}
#define MOB_RX_COMPLETED_DLCW  ((1 << RXOK)|(1 << DLCW))
// } Macro


// MOb {
void can_mob_disable(void) {
    CANCDMOB &= !(1 << CONMOB1);
    CANCDMOB &= !(1 << CONMOB0);
}

void can_mob_tx_mode(void) {
    CANCDMOB &= !(1 << CONMOB1);
    CANCDMOB |=  (1 << CONMOB0);
}

void can_mob_rx_mode(void) {
    CANCDMOB |=  (1 << CONMOB1);
    CANCDMOB &= !(1 << CONMOB0);
}

void can_mob_rx_fb_mode(void) {
    CANCDMOB |= (1 << CONMOB0);
    CANCDMOB |= (1 << CONMOB1);
}

void can_mob_disable_interrupts(void) {
    CANSTMOB = 0;
}

uint8_t can_mob_get_tx_status(void) {
    return CANSTMOB & (1 << TXOK);
}

uint8_t can_mob_get_rx_status(void) {
    return (CANSTMOB & MOB_RX_COMPLETED_DLCW);
}
// } MOb

void can_enable_mode(void) {
    CANGCON |= (1 << ENASTB);
    while( (CANGSTA & (1 << ENFG)) == 0 );
}

void can_set_mob(uint8_t mob) {
    CANPAGE = (mob << 4);
}

uint8_t can_get_mob(void) {
    return (CANPAGE >> 4);
}

void can_init_mobs(void) {
    uint8_t i;

    for(i = 0; i < 15; ++i) {
        can_set_mob(i);
        can_mob_disable();
        can_mob_disable_interrupts();
    }
}

void can_set_bt(uint8_t bt1, uint8_t bt2, uint8_t bt3) {
    CANBT1 = bt1; CANBT2 = bt2; CANBT3 = bt3;
}

void can_config_rate(void) {
//    can_set_bt(
//            0x26,
//            0x04,
//            0x13
//            );
    can_set_bt(0x06, 0x0c, 0x37);
}

void can_init(void) {
    can_init_mobs();
    can_enable_mode();
}

int can_get_free_mob(void) {
    uint8_t i;
    int res = -1;
    uint16_t canen = CANEN2;
    canen |= (CANEN1 << 8);

    for(i = 0; i < 15; ++i) {
        if( (canen & (1 << i)) == 0 ) {
            res = i;
            break;
        }
    }

    return res;
}
void led_toggle(uint8_t, uint32_t);
int can_send_frame(const can_frame * msg) {
    if(msg == 0) return -1;

    uint8_t i;
    int mob = can_get_free_mob();
    if(mob != -1) {
        can_set_mob(mob);
        CAN_CLEAR_MOB();

        if(msg->ctrl.ide) { CAN_SET_EXT_ID(msg->id.ext); }
        else { CAN_SET_STD_ID(msg->id.std); }

        CANPAGE &= ~(0x07);
        for(i = 0; i < msg->dlc; ++i) {
            CANMSG = msg->data[i];
        }
        CANCDMOB |= (msg->dlc & CAN_DLC_MASK);

        if(msg->ctrl.rtr) { CAN_SET_RTR(); }
        else { CAN_CLEAR_RTR(); }

        can_mob_tx_mode();
    }

    return mob;
}

int can_en_receive_mob(void) {
    int mob = can_get_free_mob();
    if(mob != -1) {
        can_set_mob(mob);
        can_mob_rx_mode();
    }

    return mob;
}

void can_read_frame(can_frame * msg) {
    uint8_t i;

    if (CAN_GET_IDE()) {
        msg->ctrl.ide = 1;
        CAN_GET_EXT_ID(msg->id.ext);
    } else {
        msg->ctrl.ide = 0;
        CAN_GET_STD_ID(msg->id.std);
    }

    msg->ctrl.rtr = CAN_GET_RTR();
    msg->dlc = CAN_GET_DLC();

    for(i = 0; i < msg->dlc; ++i) {
        msg->data[i] = CANMSG;
    }
}

int can_receive_frame(can_frame * msg) {
    if(msg == 0) return -1;

    int mob = can_en_receive_mob();
    if(mob != -1) {
        while(can_mob_get_rx_status() != 1) { _delay_ms(10); }

        can_read_frame(msg);
    }

    return mob;
}
