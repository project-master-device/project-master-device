#include <util/delay.h>

#include "mcp2515_bittime.h"
#include "mcp.h"

void mcp_init() {
    spi_init();
    SPIDDR |= (1 << MCP_SS);
    SPIPORT |= (1 << MCP_SS);
}

void mcp_select() {
    SPIPORT &= ~(1 << MCP_SS);
}

void mcp_unselect() {
    SPIPORT |= (1 << MCP_SS);
}

uint8_t mcp_exec_instruction(uint8_t instr) {
    uint8_t res;

    mcp_select();
    spi_transfer_one(instr);
    res = spi_transfer_one(0);
    mcp_unselect();

    return res;
}

uint8_t mcp_read_reg(uint8_t reg) {
    uint8_t spi_buf[3];

    spi_buf[0] = MCP_INSTRUCTION_READ;
    spi_buf[1] = reg;

    mcp_select();
    spi_transfer(spi_buf, spi_buf, 3);
    mcp_unselect();

    return spi_buf[2];
}

void mcp_read_seq_reg(uint8_t reg, uint8_t * recv_buf, uint8_t len) {
    uint8_t spi_buf[len + 2];
    uint8_t i;

    spi_buf[0] = MCP_INSTRUCTION_READ;
    spi_buf[1] = reg;

    mcp_select();
    spi_transfer(spi_buf, spi_buf, len + 2);
    mcp_unselect();

    for(i = 0; i < len; ++i) {
        recv_buf[i] = spi_buf[i + 2];
    }
}

void mcp_read_rx_buffer(uint8_t rx_buf_num, uint8_t * recv_buf, uint8_t len) {
    uint8_t spi_buf[len + 1];
    uint8_t i;

    spi_buf[0] = MCP_INSTRUCTION_READ_RXB(rx_buf_num);

    mcp_select();
    spi_transfer(spi_buf, spi_buf, len + 1);
    mcp_unselect();

    for(i = 0; i < len; ++i) {
        recv_buf[i] = spi_buf[i + 1];
    }
}

void mcp_write_reg(uint8_t reg, uint8_t val) {
    uint8_t spi_buf[3];

    spi_buf[0] = MCP_INSTRUCTION_WRITE;
    spi_buf[1] = reg;
    spi_buf[2] = val;

    mcp_select();
    spi_transfer(spi_buf, spi_buf, 3);
    mcp_unselect();
}

void mcp_write_seq_reg(uint8_t begin_reg, const uint8_t * send_buf, uint8_t len) {
    uint8_t spi_buf[len + 2];
    uint8_t i;

    spi_buf[0] = MCP_INSTRUCTION_WRITE;
    spi_buf[1] = begin_reg;

    // mcp2515 has auto-increment of address-pointer
    for(i = 0; i < len; ++i) {
        spi_buf[i + 2] = send_buf[i];
    }

    mcp_select();
    spi_transfer(spi_buf, spi_buf, len + 2);
    mcp_unselect();
}

void mcp_modify_reg(uint8_t reg, uint8_t mask, uint8_t val) {
    uint8_t spi_buf[4];

    spi_buf[0] = MCP_INSTRUCTION_BIT_MODIFY;
    spi_buf[1] = reg;
    spi_buf[2] = mask;
    spi_buf[3] = val;

    mcp_select();
    spi_transfer(spi_buf, spi_buf, 4);
    mcp_unselect();
}

void mcp_reset(void) {
    mcp_select();
    spi_transfer_one(MCP_INSTRUCTION_RESET);
    mcp_unselect();
    _delay_ms(5);
}

uint8_t mcp_check_reg(uint8_t reg, uint8_t mask, uint8_t val) {
    uint8_t i;

    for(i = 0; i < 100; ++i) {
        uint8_t r = mcp_read_reg(reg);

        if( (r & mask) == val ) {
            return 1;
        }

        _delay_ms(1);
    }

    return 0;
}

uint8_t mcp_check_reset_result(void) {
    uint8_t i, r;
    for(i = 0; i < 100; ++i) {
        r = mcp_read_reg(MCP_CANSTAT);

        if((r & MCP_CANCTRL_REQOP_MASK) == MCP_CANCTRL_REQOP_CONF) {
            return 1;
        }

        _delay_ms(1);
    }

    return 0;
}

uint8_t mcp_probe(void) {
    uint8_t stat, ctrl;
    mcp_reset();

    if(!mcp_check_reset_result()) {
        return 0;
    }

    stat = mcp_read_reg(MCP_CANSTAT) & 0xEE;
    ctrl = mcp_read_reg(MCP_CANCTRL) & 0x17;

    return stat == 0x80 && ctrl == 0x07;
}

uint8_t mcp_set_normal_mode(void) {
    //TODO: use modify register instruction
    uint8_t ctrl = mcp_read_reg(MCP_CANCTRL);

    ctrl &= ~(1 << 7);
    ctrl &= ~(1 << 6);
    ctrl &= ~(1 << 5);

    mcp_write_reg(MCP_CANCTRL, ctrl);

    return mcp_check_reg(MCP_CANCTRL, MCP_CANCTRL_REQOP_MASK, MCP_CANCTRL_REQOP_NORMAL);
}

uint8_t mcp_set_sleep_mode(void) {
    //TODO: use modify register instruction
    uint8_t ctrl = mcp_read_reg(MCP_CANCTRL);

    ctrl &= ~(1 << 7);
    ctrl &= ~(1 << 6);
    ctrl |=  (1 << 5);

    mcp_write_reg(MCP_CANCTRL, ctrl);

    return mcp_check_reg(MCP_CANCTRL, MCP_CANCTRL_REQOP_MASK, MCP_CANCTRL_REQOP_SLEEP);
}

uint8_t mcp_set_loopback_mode(void) {
    //TODO: use modify register instruction
    uint8_t ctrl = mcp_read_reg(MCP_CANCTRL);

    ctrl &= ~(1 << 7);
    ctrl |=  (1 << 6);
    ctrl &= ~(1 << 5);

    mcp_write_reg(MCP_CANCTRL, ctrl);

    return mcp_check_reg(MCP_CANCTRL, MCP_CANCTRL_REQOP_MASK, MCP_CANCTRL_REQOP_LOOPBACK);
}

uint8_t mcp_set_listen_only_mode(void) {
    //TODO: use modify register instruction
    uint8_t ctrl = mcp_read_reg(MCP_CANCTRL);

    ctrl &= ~(1 << 7);
    ctrl |=  (1 << 6);
    ctrl |=  (1 << 5);

    mcp_write_reg(MCP_CANCTRL, ctrl);

    return mcp_check_reg(MCP_CANCTRL, MCP_CANCTRL_REQOP_MASK, MCP_CANCTRL_REQOP_LISTEN_ONLY);
}

uint8_t mcp_set_configuration_mode(void) {
    //TODO: use modify register instruction
    uint8_t ctrl = mcp_read_reg(MCP_CANCTRL);

    ctrl |=  (1 << 7);
    ctrl &= ~(1 << 6);
    ctrl &= ~(1 << 5);

    mcp_write_reg(MCP_CANCTRL, ctrl);

    return mcp_check_reg(MCP_CANCTRL, MCP_CANCTRL_REQOP_MASK, MCP_CANCTRL_REQOP_CONF);
}

uint8_t mcp_get_free_txbuf(uint8_t * buf_ctrl) {
    uint8_t i, ctrl;
    uint8_t ctrlregs[MCP_N_TXBUFFERS] = { MCP_TXB0CTRL, MCP_TXB1CTRL, MCP_TXB2CTRL };

    for (i = 0; i < MCP_N_TXBUFFERS; ++i) {
        ctrl = mcp_read_reg(ctrlregs[i]);

        if ( (ctrl & MCP_TXB_TXREQ_MASK) == MCP_TXB_TXREQ_NOT_PENDING ) {
            // SIDH -- start of frame
            *buf_ctrl = ctrlregs[i];

            return 1;
        }
    }
    *buf_ctrl = 0x00;

    return 0;
}

// Standart id only
void mcp_write_can_id(uint8_t buf_begin_addr, uint32_t can_id) {
    uint16_t canid;
    uint8_t id_buf[4];

    canid = (uint16_t)(can_id & 0x0FFFF);
    // SIDH
    id_buf[0] = (uint8_t) (canid / 8);
    // SIDL
    id_buf[1] = (uint8_t) ((canid & 0x07) * 32);
    // EID8
    id_buf[2] = 0;
    // EID0
    id_buf[3] = 0;

    id_buf[1] &= (uint8_t) (~(1 << 3));
    //id_buf[1] |= (uint8_t) (1 << 3);

    mcp_write_seq_reg(buf_begin_addr, id_buf, 4);
}

// Standart id only
void mcp_read_can_id(uint8_t buf_begin_addr, uint32_t * can_id) {
    uint8_t id_buf[4];

    mcp_read_seq_reg(buf_begin_addr, id_buf, 4);

    *can_id = (id_buf[0] << 3) + (id_buf[1] >> 5);
}

// Standart data frame only
void mcp_write_frame(uint8_t buf_begin_addr, const can_frame * msg) {
    uint8_t dlc = msg->can_dlc;

    mcp_write_seq_reg(buf_begin_addr + 5, msg->data, dlc);

    mcp_write_can_id(buf_begin_addr, msg->can_id);

    if(msg->can_rtr == 1)  dlc |= MCP_DLC_RTR_MASK;

    mcp_write_reg(buf_begin_addr + 4, dlc);
}

void mcp_read_frame(uint8_t buf_begin_addr, can_frame * msg) {
    uint8_t buf_ctrl;

    mcp_read_can_id(buf_begin_addr, &(msg->can_id));

    buf_ctrl = mcp_read_reg(buf_begin_addr - 1);
    msg->can_dlc = mcp_read_reg(buf_begin_addr + 4);

    if(buf_ctrl & MCP_RXB_RXRTR_MASK) {
        msg->can_rtr = 1;
    } else {
        msg->can_rtr = 0;
    }

    //msg->can_dlc &= MCP_DLC_DLC_MASK;
    mcp_read_seq_reg(buf_begin_addr + 5, msg->data, msg->can_dlc);
}

/*
void mcp_read_frame1(uint8_t buf_begin_addr, can_frame * msg) {
    uint8_t rx_buf_num;
    uint8_t rx_buf[13];

    switch(buf_begin_addr) {
        case MCP_RXB0_BEGIN:
            rx_buf_num = MCP_RXB0;
            break;
        case MCP_RXB1_BEGIN:
            rx_buf_num = MCP_RXB1;
            break;
    }

    mcp_read_rx_buffer(rx_buf_num, rx_buf, 13);

    buf_ctrl = mcp_read_reg(buf_begin_addr - 1);
    //todo: later
}
*/

void mcp_start_transmit(uint8_t buf_ctrl) {
    mcp_modify_reg(buf_ctrl, MCP_TXB_TXREQ_MASK, MCP_TXB_TXREQ_PENDING);
}

uint8_t mcp_read_status(void) {
    return mcp_exec_instruction(MCP_INSTRUCTION_READ_STATUS);
}

uint8_t mcp_config_rate(uint8_t can_speed) {
    uint8_t set, cfg1, cfg2, cfg3;
    
    switch (can_speed) {
        case (MCP_RATE_125KBPS) :
            cfg1 = MCP_4MHz_125kBPS_CFG1 ;
            cfg2 = MCP_4MHz_125kBPS_CFG2 ;
            cfg3 = MCP_4MHz_125kBPS_CFG3 ;
            set = 1;
            break;
        case (MCP_RATE_20KBPS) :
            cfg1 = MCP_4MHz_20kBPS_CFG1 ;
            cfg2 = MCP_4MHz_20kBPS_CFG2 ;
            cfg3 = MCP_4MHz_20kBPS_CFG3 ;
            set = 1;
            break;
        default:
            set = 0;
            break;
    }

    set = 1;
    cfg1 = 0x01; cfg2 = 0xb5; cfg3 = 0x01;
   
    if (set) {
        mcp_write_reg(MCP_CNF1, cfg1);
        mcp_write_reg(MCP_CNF2, cfg2);
        mcp_write_reg(MCP_CNF3, cfg3);

        return 1;
    }
    
    return 0;
}

