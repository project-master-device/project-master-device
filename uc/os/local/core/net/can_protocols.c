// 08.02.2011 - C
// 28.04.2011

/*
Copyright (C) 2011 by <Project Master Device>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "can_protocols.h"

/*
uint64_t mask_by_len(const unsigned int length) {
	return 2^length - 1
}
 */

/*-----------------------------------------------------------------------------------------------*/
//< 6/24 | 5 >

#define FRAME_HEAD_VERSION_OFFSET 			0			// 0
#define FRAME_HEAD_VERSION_MASK 			0x1f		// 5b
#define FRAME_HEAD_HWA_OFFSET 				5			// 0+5
#define FRAME_HEAD_HWA_MASK					0x3f		// 6b
#define FRAME_HEAD_HWA_MASK_EXTENDED 		0xffffff	// 24b

int frame_head_pack(uint32_t* packed, const uint8_t version, const uint32_t hw_addr, const uint8_t ide) {
#ifdef CAN_PROTOCOLS_VALIDATE
	uint32_t hwa_mask = FRAME_HEAD_HWA_MASK;
	if (ide) {
		hwa_mask = FRAME_HEAD_HWA_MASK_EXTENDED;
	}
	if (version > FRAME_HEAD_VERSION_MASK) return 1;
	if (hw_addr > hwa_mask) return 1;
#endif
	uint32_t ver_b = version << FRAME_HEAD_VERSION_OFFSET;
	uint32_t hwa_b = hw_addr << FRAME_HEAD_HWA_OFFSET;
	*packed = ver_b | hwa_b;
	return 0;
}
frame_head_t frame_head_unpack(const uint32_t can_id, const uint8_t ide) {
	frame_head_t unpacked;
	uint32_t hwa_mask = FRAME_HEAD_HWA_MASK;
	if (ide) {
		hwa_mask = FRAME_HEAD_HWA_MASK_EXTENDED;
	}
	unpacked.version = (can_id >> FRAME_HEAD_VERSION_OFFSET) & FRAME_HEAD_VERSION_MASK;
	unpacked.hw_addr = (can_id >> FRAME_HEAD_HWA_OFFSET) & hwa_mask;
	return unpacked;
}

/*-----------------------------------------------------------------------------------------------*/

// < 10 | 1 | 4 >
#define ACC_BASE_PORT_OFFSET				0		// 0b
#define ACC_BASE_PORT_MASK					0xf		// 4b
#define ACC_BASE_SMB_OFFSET					4		// 0+4
#define ACC_BASE_SMB_MASK					0x1		// 1b
#define ACC_BASE_ID_OFFSET					5		// 0+4+1
#define ACC_BASE_ID_MASK					0x3ff	// 10b
#define ACC_BASE_MASK						0x7fff	// 15b
int lvl2_acc_base_pack(uint16_t* packed, uint8_t port, const uint8_t smb, const uint16_t id) {
#ifdef CAN_PROTOCOLS_VALIDATE
	if (port > ACC_BASE_PORT_MASK) return 1;
	if (smb > ACC_BASE_SMB_MASK) return 1;
	if (id  > ACC_BASE_ID_MASK) return 1;
#endif
	uint16_t port_b = port << ACC_BASE_PORT_OFFSET;
	uint16_t smb_b = smb << ACC_BASE_SMB_OFFSET;
	uint16_t id_b  = id  << ACC_BASE_ID_OFFSET;
	*packed = (port_b | smb_b | id_b);// & ACC_BASE_MASK;
	return 0;
}
lvl2_acc_base_t lvl2_acc_base_unpack(const uint16_t packed) {
	lvl2_acc_base_t unpacked;
	uint16_t* header = (uint16_t*)&packed;
	unpacked.port = (*header >> ACC_BASE_PORT_OFFSET) & ACC_BASE_PORT_MASK;
	unpacked.smb = (*header >> ACC_BASE_SMB_OFFSET) & ACC_BASE_SMB_MASK;
	unpacked.id  = (*header >> ACC_BASE_ID_OFFSET)  & ACC_BASE_ID_MASK;
	return unpacked;
}

// < [0;64-(9-x)] | 8 | 1 | x >
#define SEGM_FNL_LFB_OFFSET					0		// 0
#define SEGM_FNL_LFB_MASK					0x1		// 1b
#define SEGM_FNL_NUM_OFFSET					1		// 1
#define SEGM_FNL_NUM_MASK					0xff	// 8b
#define SEGM_FNL_MASK						0x1ff	// 9b
int lvl_segm_fnl_pack(can_frame_data_t* packed, const uint8_t lfb, const uint8_t num, const uint8_t* data, const uint8_t data_len) {
#ifdef CAN_PROTOCOLS_VALIDATE
	if (data_len > CAN_NET_SEGM_FNL_DATALEN_B) return 1;
	if (lfb > SEGM_FNL_LFB_MASK) return 1;
	if (num > SEGM_FNL_NUM_MASK) return 1;
#endif
	uint16_t lfb_b = lfb << SEGM_FNL_LFB_OFFSET;
	uint16_t num_b = num << SEGM_FNL_NUM_OFFSET;
	uint16_t header = (lfb_b | num_b) << CAN_NET_SEGM_FNL_OFFSET;// & SEGM_FNL_MASK
	// put header:
	memcpy(&packed->itself[CAN_NET_SEGM_FNL_OFFSET_B], (uint8_t*)&header, CAN_NET_SEGM_FNL_LEN_B);
	// put data:
	memcpy(&packed->itself[CAN_NET_LVL2_STD_HEADLEN_B], data, data_len);
	packed->len = data_len + CAN_NET_LVL2_STD_HEADLEN_B;
	return 0;
}
lvl_segm_fnl_t lvl_segm_fnl_unpack(const can_frame_data_t* packed) {
	lvl_segm_fnl_t unpacked;
	// retrieve data:
	//uint16_t header = *((uint16_t*)(&packed->itself[CAN_NET_SEGM_FNL_OFFSET_B])) >> CAN_NET_SEGM_FNL_OFFSET;
	// kostyil for arm compiler:
	uint16_t header = (packed->itself[CAN_NET_SEGM_FNL_OFFSET_B + 1] << 8) | (packed->itself[CAN_NET_SEGM_FNL_OFFSET_B]);
	header >>= CAN_NET_SEGM_FNL_OFFSET;
	unpacked.lfb = (header >> SEGM_FNL_LFB_OFFSET) & SEGM_FNL_LFB_MASK;
	unpacked.num = (header >> SEGM_FNL_NUM_OFFSET) & SEGM_FNL_NUM_MASK;
	// retrieve data:
	unpacked.data.len = packed->len - CAN_NET_LVL2_STD_HEADLEN_B;
	memcpy(unpacked.data.itself, &packed->itself[CAN_NET_LVL2_STD_HEADLEN_B], unpacked.data.len);
	return unpacked;
}

/*-----------------------------------------------------------------------------------------------*/
//< 0-40 | 9 | 15 >

#define LVL2_STD_BASE_OFFSET				0
#define LVL2_STD_BASE_MASK					ACC_BASE_MASK
// kostyil, TODO: make new protocol: <DATA-32; SEGM:(num-15, lfb-1); BASE:(id-8, smb-1, port-7)>
#define LVL2_STD_BASE_ADD_MASK				0x8000

int lvl2_std_pack(can_frame_data_t* frame_data, const uint16_t base) {
#ifdef CAN_PROTOCOLS_VALIDATE
	if (base > LVL2_STD_BASE_MASK) return 1;
//	if (segm ...) return 1;
#endif
	uint16_t header = base << LVL2_STD_BASE_OFFSET;
	// LVL2_ACC_BASE_LEN=15 < 16, so we need to take that bit from data
	header = header | (*((uint16_t*)frame_data->itself) & LVL2_STD_BASE_ADD_MASK);

	// put header:
	memcpy(&frame_data->itself, (uint8_t*)&header, CAN_NET_ACC_BASE_LEN_B);
	// does not care about segmentation!
	return 0;
}
lvl2_std_t lvl2_std_unpack(const can_frame_data_t* packed) {
	lvl2_std_t unpacked;
	// retrieve header:
	uint16_t base_ = *((uint16_t*)(packed->itself)) & ACC_BASE_MASK;
	//bwahaha megaspell:
	//	uint16_t base_ = ( *((uint16_t*)(&packed->itself[CAN_NET_LVL2_STD_BASE_OFFSET])) >> CAN_NET_LVL2_STD_BASE_OFFSET_B ) & ACC_BASE_MASK;

	unpacked.base = lvl2_acc_base_unpack(base_);
	// retrieve data:
	unpacked.segm = lvl_segm_fnl_unpack(packed);
	return unpacked;
}

/*-----------------------------------------------------------------------------------------------*/

int lvl2_std_confirm_pack(can_frame_data_t* packed, const uint16_t base) {
#ifdef CAN_PROTOCOLS_VALIDATE
	if (base > ACC_BASE_MASK) return 1;
#endif
	memcpy(packed->itself, (uint8_t*)&base, CAN_NET_LVL2_STD_CONFIRM_SIZE_CONST);
	packed->len = CAN_NET_LVL2_STD_CONFIRM_SIZE_CONST;
	return 0;
}

lvl2_std_confirm_t lvl2_std_confirm_unpack(const can_frame_data_t* packed) {
	lvl2_std_confirm_t unpacked;
	uint16_t base_ = *((uint16_t*)(&packed->itself)) & ACC_BASE_MASK;
//	uint16_t base_ = ( *((uint16_t*)(&packed->itself[CAN_NET_LVL2_CONF_BASE_OFFSET_B])) >> CAN_NET_LVL2_CONF_BASE_OFFSET ) & ACC_BASE_MASK;
	unpacked = lvl2_acc_base_unpack(base_);
	return unpacked;
}

/*-----------------------------------------------------------------------------------------------*/

