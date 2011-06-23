#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>

typedef uint8_t Bool;

typedef struct {
  Bool rtr;
  Bool ide;
} can_ctrl_t;

typedef union {
  uint32_t ext;
  uint16_t std;
  uint8_t  tab[4];
} can_id_t;

typedef struct {
    can_ctrl_t ctrl;
    can_id_t id;
    uint8_t dlc;
    uint8_t data[8] __attribute__((aligned(8)));
} can_frame;

// MOb {
void can_mob_disable(void);
void can_mob_tx_mode(void);
void can_mob_rx_mode(void);
void can_mob_rx_fb_mode(void);
void can_mob_disable_interrupts(void);
uint8_t can_mob_get_tx_status(void);
uint8_t can_mob_get_rx_status(void);
// } MOb

void can_enable_mode(void);

void can_set_mob(uint8_t mob);
uint8_t can_get_mob(void);
int can_get_free_mob(void);

int can_send_frame(const can_frame * msg);
int can_en_receive_mob(void);
void can_read_frame(can_frame * msg);
int can_receive_frame(can_frame * msg);

void can_set_bt(uint8_t bt1, uint8_t bt2, uint8_t bt3);
void can_config_rate(void);
void can_init_mobs(void);
void can_init(void);

#endif /* CAN_H_ */
