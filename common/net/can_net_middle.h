// 19.05.2011
// 03.06.2011
// to be changed later:
// MITL

#ifndef CAN_NET_MIDDLE_H_
#define CAN_NET_MIDDLE_H_

#include "can_net_core.h"

/* defines:
		CAN_NET_CONFIRMATION	- use confirmations on msg
									(works bad when testing can_net one one machine,- only with old can_socket
*/
#define CAN_NET_CONFIRMATION

/* -------------------------------------INTERFACE_STRUCTS---------------------------------------*/

/**
 * @struct bytearr_t
 * @brief simple byte array
 */
typedef struct {
	uint8_t* itself;
	uint32_t len;
} bytearr_t;

/**
 * @struct msg_metadata_t
 * @brief header for message
 */
typedef struct {
	uint32_t hw_addr;
	uint8_t port;
	uint8_t is_system;
	uint16_t id;
} msg_metadata_t;

typedef struct {
	msg_metadata_t meta;
	bytearr_t data;
} msg_lvl2_t;

typedef void (*can_net_send_callback_t)(const int rc, msg_lvl2_t* msg);
typedef void (*can_net_recv_callback_t)(msg_lvl2_t* msg);

//and small bonus
inline void call_scb(can_net_send_callback_t send_cb, const int rc, msg_lvl2_t* msg);

/*----------------------------------------------------------------------------------------------*/

//return codes for callbacks:
#define CAN_NET_RC_NORM						0
#define CAN_NET_RC_UNKNOWN					1
#define CAN_NET_RC_CONF_TIMEOUT				2
#define CAN_NET_RC_TOO_LONG_MSG				4
#define CAN_NET_RC_BAD_DATA					100
// return codes >THIS are from driver (defined in can_drv.h => are uniq)
#define CAN_NET_RC_DRIVER_ERRS_START		10000

/* -----------------------------------CONFIRMATION_data/struct/funcs----------------------------*/

#ifdef CAN_NET_CONFIRMATION

typedef struct {
	void* next; // to use in list
	msg_lvl2_t* msg;
	uint32_t tics_left; // num of timer_tics else to wait
	can_net_send_callback_t callback;
} confirm_waiter_t;

inline void remove_confirm_waiter(confirm_waiter_t* waiter);

inline void add_confirm_waiter(msg_lvl2_t* msg, const uint32_t tics, const can_net_send_callback_t cb);

confirm_waiter_t* find_confirm_waiter(const uint8_t port, const uint32_t hwa, const uint8_t smb, const uint16_t id);

void check_confirm_waiters();

#endif

#endif /* CAN_NET_MIDDLE_H_ */
