#ifndef CAN_DRV_H_
#define CAN_DRV_H_

#include "can_drv_structs.h"
//#include "list.h"

#define CAN_DRV_RC_NORM					0
#define CAN_DRV_RC_TIMEOUT				1
#define CAN_DRV_RC_TIMER_ERR			2
#define CAN_DRV_RC_WRONG_DATA			3

typedef void (*can_send_callback_t)(const int, can_frame_t *, void *);
typedef void (*can_recv_callback_t)(can_frame_t *);

typedef uint32_t can_mask_t;

int can_init(can_speed_t can_speed, can_send_callback_t send_callback_, can_recv_callback_t recv_callback_,
		uint32_t send_timeout_real_us , can_mask_t masked_id, can_mask_t mask);
int can_send(can_frame_t * can_frame, void * context);

void can_set_mask(can_mask_t masked_id, can_mask_t mask);
int can_set_speed(can_speed_t can_speed);

#ifdef CONTIKI
#include "process.h"

PROCESS_NAME(can_drv_process);

#endif

#endif /* CAN_DRV_H_ */
