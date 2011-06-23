// 08.02.2011 - C
// 12.04.2011
//
/*
 * Copyright (C) 2011 by <Project Master Device>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

/**
 * @file can_protocols
 * @brief protocol to use in can network - v0.3
 * CAN_PROTOCOLS_VALIDATE: validate before _pack, it protects from setting values between
 * MAX for num of bits in protocol and MAX for data type
 * TODO: for CANDATA: < DATA-[0;32] |{ num-15 | lfb-1 }|{ id-8 | smb-1 | port-7} >
 */

#ifndef CAN_LOCAL_PROTOCOLS_H
#define CAN_LOCAL_PROTOCOLS_H

#include <string.h>
#include "can_drv_structs.h"
//#include "can_drv.h"

#define CAN_PROTOCOLS_VALIDATE

/* ----------------------------------------------------------------------------------------------*/

/**
 * @struct frame_head_t
 * @brief lvl1, (CONSTANT part)
 *
 * level_1: 14/32 + 0-64 bit
 * < 6/24 | 5 | 1 | 1 | 1 > < 0-64 >
 * < hw_address | version | err | rtr | ide | > < data >
 *
 * controller = slave, hw_addr = slave address (shortcut: "hwa")
 * only slaves have hw_addr:
 * slave  --> master frame: hw_addr of slave
 * master --> slave  frame: hw_addr of slave
 *
 * CAN_ID      < protocol_version | hw_addr >
 *             < 6/24 | 5 >
 *             ( normal-11bit / extended-29bit )
 * CAN_DATA    <data>
 *             <0-64>
 *             (up to 8 Bytes)
 */
typedef struct {
	uint8_t version;	///< lvl2 protocol version
	uint32_t hw_addr;	///< slave address
} frame_head_t;
int frame_head_pack(uint32_t* packed, const uint8_t version, const uint32_t hw_addr, const uint8_t ide);
frame_head_t frame_head_unpack(const uint32_t can_id, const uint8_t ide);
#define CAN_NET_LVL1_MAX_HWA_NOT_EXT		0x3f

// TODO:? rename packed to dst

/* ----------------------------------------------------------------------------------------------*/
/* -----lvl2 protocols - in CAN_DATA-------------------------------------------------------------*/

/**
 * @struct lvl2_acc_base_t
 * @brief accessory for real lvl2 protocols - base information for frame
 *
 * < 10 |  1  |  4  >
 * < id | smb | port >
 * usually, there is no need for id+smb in frames after first,
 * but saving one byte doesn't worth making new acc protocol
 */
typedef struct {
	uint8_t port;		///< application code, works like port in unix
	uint8_t smb;		///< system message bit
	uint16_t id;		///< module id or system message type
} lvl2_acc_base_t;
int lvl2_acc_base_pack(uint16_t* packed, const uint8_t port, const uint8_t smb, const uint16_t id);
lvl2_acc_base_t lvl2_acc_base_unpack(const uint16_t packed);
#define CAN_NET_ACC_BASE_LEN				15
#define CAN_NET_ACC_BASE_LEN_B				2


/* The CAN bus protocol assures with an extreme high probability that no messages
are falsified or lost. But it is possible that a message is doubled by
a single bit error near the end of End of Frame (EOF)!
Therefore, if CAN bus is used in a disturbed environment:
    * don´t use toggle messages
    * don´t transmit messages carrying relative data (like angle increments or delta counts)
    * use protected protocols or sequence numbers for data or program segmentation
p = 7.8 * 10^-6, when p(1 bit err) = 10^-3 */


/**
 * @struct lvl_segm_fnl_t
 * @brief segmentation: Forward Numbering, Last frame bit segmentation
 *
 * < [0;64-(9-x)] |  8  |  1  |     X      >
 * <     data     | num | lfb | other_lvl2 >
 * packs segment straight to frame_data, keeping X bit for real lvl2 protocol header
 * (just to make it easier to separate acc_base and segment packing)
 */
typedef struct {
	uint8_t lfb;			///< last frame bit
	uint8_t num;			///< frame number in message
	can_frame_data_t data;	///< data
} lvl_segm_fnl_t;
int lvl_segm_fnl_pack(can_frame_data_t* frame_data, const uint8_t lfb, const uint8_t num, const uint8_t* data, const uint8_t datalen);
lvl_segm_fnl_t lvl_segm_fnl_unpack(const can_frame_data_t* packed);
// calculated to use lvl_segm_fnl with acc_base:
#define CAN_NET_SEGM_FNL_LEN					9
#define CAN_NET_SEGM_FNL_LEN_B					2
#define CAN_NET_SEGM_FNL_MAX_NUM				0xff	// 255

// x = CAN_NET_ACC_BASE_LEN = 15				//(megabrainfuck):
#define CAN_NET_SEGM_FNL_OFFSET_B				(CAN_NET_ACC_BASE_LEN / 8)
#define CAN_NET_SEGM_FNL_OFFSET					(CAN_NET_ACC_BASE_LEN - CAN_NET_SEGM_FNL_OFFSET_B * 8)
#define CAN_NET_LVL2_STD_HEADLEN_B				(CAN_NET_SEGM_FNL_LEN + CAN_NET_ACC_BASE_LEN) / 8		//WARNING!
#define CAN_NET_SEGM_FNL_DATALEN_B				(8 - CAN_NET_LVL2_STD_HEADLEN_B)

/**
 * @struct lvl2_std_t
 * @brief level 2 standard protocol - ver.1
 *
 * < 0-40 |     9     |  15  >
 * < data | partition | base >
 * simultaneously only one master.port<-->slave msg can be transmitted
 * (only port & hw_addr are necessary in every frame)
 */
typedef struct {
	lvl2_acc_base_t base;	///< base inf
	lvl_segm_fnl_t segm;	///< segment + data
} lvl2_std_t;
int lvl2_std_pack(can_frame_data_t* frame_data, const uint16_t base);
lvl2_std_t lvl2_std_unpack(const can_frame_data_t* packed);
//#define CAN_NET_LVL2_STD_DATALEN_B				CAN_NET_SEGM_FNL_DATALEN_B
//#define CAN_NET_LVL2_STD_SEGM_OFFSET_B			CAN_NET_ACC_BASE_LEN_B
//#define CAN_NET_LVL2_STD_SEGM_MAX_NUM				CAN_NET_SEGM_FNL_MAX_NUM
//#define CAN_NET_LVL2_STD_BASE_OFFSET_B			0
//#define CAN_NET_LVL2_STD_BASE_OFFSET				0

/* ----------------------------------------------------------------------------------------------*/

/**
 * @struct lvl2_confirm_t
 * @brief level 2 confirmation frame - sending it confirms that you received message
 *
 * synchronous confirmation
 */
//typedef struct {
//	lvl2_acc_base_t base;	///< base inf
//} lvl2_std_confirm_t;
typedef lvl2_acc_base_t lvl2_std_confirm_t;

int lvl2_std_confirm_pack(can_frame_data_t* packed, const uint16_t base);
lvl2_std_confirm_t lvl2_std_confirm_unpack(const can_frame_data_t* packed);
//#define CAN_NET_LVL2_CONF_BASE_OFFSET_B					0
//#define CAN_NET_LVL2_CONF_BASE_OFFSET						0
#define CAN_NET_LVL2_STD_CONFIRM_SIZE_CONST					2

/* ----------------------------------------------------------------------------------------------*/

#endif // CAN_LOCAL_PROTOCOLS_H
