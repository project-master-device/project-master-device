# 24.11.2010
# 15.12.2010

#TODO: licence - MIT

import struct

def mask_by_len(length):
    mask = 0
    for i in range(0, length):
        mask += 2**i
    return mask

#TODO (low priority): protogen, < 8bytes length

#v0.2 - "big-endian" element order

# lvl1 protocols: 1-common(python only recieves), 2-short(python only sends)

# most significant element first (rightward)
# < data-lvl3 | lvl2 | lvl1 >

# TO CONTROLLER:

# multi-message recieving is based on last_frame_bit => msgs should be sended in order

class out_lvl2_common:
    '''
    IN level 2 protocol, msg: 57 bits
    < 32 | 1 | 4 | 20 >
    < data | last_frame_bit | device_id | hw_addr >
    '''
    data_mask = mask_by_len(53)
    data_offset = 4 + 1 + 4
    lfb_mask = mask_by_len(1)
    lfb_offset = 4
    device_id_mask = mask_by_len(4)
    device_id_offset = 0
    hw_addr_mask = mask_by_len(4)
    hw_addr_offset = 0
    
    version = 1
    
    @classmethod
    def pack_msg(cls, hw_addr, device_id, lfb, data):
        hw_addr_b = (hw_addr & cls.hw_addr_mask) << cls.hw_addr_offset
        device_id_b = (device_id & cls.device_id_mask) << cls.device_id_offset
        lfb_b = (data & cls.lfb_mask) << cls.lfb_offset
        data_b = (data & cls.data_mask) << cls.data_offset
        msg_int = (data_b | lfb_b | device_id_b | hw_addr_b)
        return (msg_int, )


# Controller Area Network Identifier structure
# bit 0-28	: CAN identifier (11/29 bit)
# bit 29	: error frame flag (0 = data frame, 1 = error frame)
# bit 30	: remote transmission request flag (1 = rtr frame)
# bit 31	: frame format flag (0 = standard 11 bit, 1 = extended 29 bit)
class out_service_inf:
    '''
    OUT service information: 32 bits
    < 29 | 1 | 1 | 1 >
    < CAN_id | ERR | RTR | EFF >
    '''
    mask_32b = mask_by_len(32)
    canid_mask = mask_by_len(29)
    canid_offset = 1 + 1 + 1
    err_mask = mask_by_len(1)
    err_offset = 1 + 1
    rtr_mask = mask_by_len(1)
    rtr_offset = 1
    eff_mask = mask_by_len(1)
    eff_offset = 0
    
    @classmethod
    def pack_msg(cls, canid, eff, rtr, err):
        canid_b = (canid & cls.canid_mask) << cls.canid_offset
        err_b = (err & cls.err_mask) << cls.err_offset
        rtr_b = (rtr & cls.rtr_mask) << cls.rtr_offset
        eff_b = (eff & cls.eff_mask) << cls.eff_offset
        serv_inf = (canid_b | err_b | rtr_b | eff_b) & cls.mask_32b
        return (serv_inf, )


class out_lvl1:
    '''
    OUT level 1 protocol, msgs: 32 + 64 bits
    < 32 > < 57 | 7 >
    < out_service_inf > < data | version >
    '''
    data_mask = mask_by_len(57)
    data_offset = 7
    version_mask = mask_by_len(7)
    version_offset = 0
    
    # flags const:
    err = 0
    rtr = 0
    eff = 1
    
    @classmethod
    def pack_msg(cls, canid, version, data):
        service_inf_int = out_service_inf.pack_msg(canid, cls.eff, cls.rtr, cls.err)[0]
        version_b = (version & cls.version_mask) << cls.version_offset
        data_b = (data & cls.data_mask) << cls.data_offset
        msg_int = (data_b | version_b)
        #TODO: pack msg int to str here
        return (service_inf_int, msg_int)

# TO NODE:

class in_lvl3_32ints:
    '''
    IN level 3 protocol, msg: 32+ bits
    < 32+ >
    < int+ >
    '''
    element_offset = 32
    
    @classmethod
    def unpack(cls, msg_ints):
        result = 0
        for i in range(0, len(msg_ints)):
            result += msg_ints[i] << (i * cls.element_offset)
        return (result, )


class in_lvl2_common:
    '''
    IN level 2 protocol, msg: 57 bits
    < 32 | 1 | 4 | 20 >
    < data | last_frame_bit | device_id | hw_addr >
    '''
    data_mask = mask_by_len(32)
    data_offset = 20 + 4 + 1
    lfb_mask = mask_by_len(1)
    lfb_offset = 20 + 4
    device_id_mask = mask_by_len(4)
    device_id_offset = 20
    hw_addr_mask = mask_by_len(20)
    hw_addr_offset = 0
    
    @classmethod
    def unpack_msg(cls, msg_int):
        hw_addr = (msg_int >> cls.hw_addr_offset) & cls.hw_addr_mask
        device_id = (msg_int >> cls.device_id_offset) & cls.device_id_mask
        lfb = (msg_int >> cls.lfb_offset) & cls.lfb_mask
        data = (msg_int >> cls.data_offset) & cls.data_mask
        return (hw_addr, device_id, lfb, data)


class in_lvl2_confirmation:
    '''
    IN level 2 protocol, msg: 57 bits
    < 33 | 4 | 20 >
    < data | device_id | hw_addr >
    '''
    data_mask = mask_by_len(33)
    data_offset = 20 + 4
    device_id_mask = mask_by_len(4)
    device_id_offset = 20
    hw_addr_mask = mask_by_len(20)
    hw_addr_offset = 0
    
    @classmethod
    def unpack_msg(cls, msg_int):
        hw_addr = (msg_int >> cls.hw_addr_offset) & cls.hw_addr_mask
        device_id = (msg_int >> cls.device_id_offset) & cls.device_id_mask
        data = (msg_int >> cls.data_offset) & cls.data_mask
        return (hw_addr, device_id, data)


class in_lvl1:
    '''
    IN level 1 protocol, msg: 64 bits
    < 57 | 7 >
    < data | protocol_version >
    '''
    data_mask = mask_by_len(57)
    data_offset = 7
    ver_mask = mask_by_len(7)
    ver_offset = 0
    
    @classmethod
    def unpack_msg(cls, msg_int):
        version = (msg_int >> cls.ver_offset) & cls.ver_mask
        data = (msg_int >> cls.data_offset) & cls.data_mask
        return (version, data)
        #return cls.protocols[version].unpack_msg(data)
