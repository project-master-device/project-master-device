#include <avr/common.h>
#include <avr/io.h>

#include "spi.h"
#include "can.h"

#ifndef MCP_SS
#define MCP_SS SPISS
#endif

#define MCP_INSTRUCTION_WRITE           0x02
#define MCP_INSTRUCTION_READ            0x03
#define MCP_INSTRUCTION_BIT_MODIFY      0x05
#define MCP_INSTRUCTION_LOAD_TXB(n)     (0x40 + 2 * (n))
#define MCP_INSTRUCTION_READ_RXB(n)     (((n) == 0) ? 0x90 : 0x94)
#define MCP_INSTRUCTION_RESET           0xC0
#define MCP_INSTRUCTION_READ_STATUS     0xA0

#define MCP_N_TXBUFFERS 3

//TODO tx buffers begin define
#define MCP_TXB0CTRL                     0x30
#define MCP_TXB1CTRL                     0x40
#define MCP_TXB2CTRL                     0x50
#   define MCP_TXB_TXREQ_MASK            0x08
#   define MCP_TXB_TXREQ_PENDING         0x08
#   define MCP_TXB_TXREQ_NOT_PENDING     0x00
#   define MCP_TXB_ABTF  0x40
#   define MCP_TXB_MLOA  0x20
#   define MCP_TXB_TXERR 0x10
#   define MCP_TXB_TXREQ 0x08

#define MCP_RXB0 0
#define MCP_RXB1 1
#define MCP_RXB0CTRL 0x60
#define MCP_RXB1CTRL 0x70
#define MCP_RXB0SIDH 0x61   
#define MCP_RXB1SIDH 0x71
#define MCP_RXB0_BEGIN MCP_RXB0SIDH
#define MCP_RXB1_BEGIN MCP_RXB1SIDH
#   define MCP_RXB_RXRTR_MASK 0x08
#   define MCP_RXBCTRL_RXM_MASK 0x60

#define MCP_DLC_RTR_MASK                0x40
#define MCP_DLC_DLC_MASK                0x0F

#define MCP_CANSTAT                     0x0e
#define MCP_CANCTRL                     0x0f
#  define MCP_CANCTRL_REQOP_MASK        0xe0
#  define MCP_CANCTRL_REQOP_CONF        0x80
#  define MCP_CANCTRL_REQOP_LISTEN_ONLY 0x60
#  define MCP_CANCTRL_REQOP_LOOPBACK    0x40
#  define MCP_CANCTRL_REQOP_SLEEP       0x20
#  define MCP_CANCTRL_REQOP_NORMAL      0x00
#  define MCP_CANCTRL_OSM               0x08
#  define MCP_CANCTRL_ABAT              0x10

#define MCP_CANINTF                         0x2c
#  define MCP_CANINTF_MERRF                 0x80
#  define MCP_CANINTF_WAKIF                 0x40
#  define MCP_CANINTF_ERRIF                 0x20
#  define MCP_CANINTF_TX2IF                 0x10
#  define MCP_CANINTF_TX1IF                 0x08
#  define MCP_CANINTF_TX0IF                 0x04
#  define MCP_CANINTF_RX1IF                 0x02
#  define MCP_CANINTF_RX0IF                 0x01

#define MCP_CANINTE       0x2b
#  define MCP_CANINTE_MERRE 0x80
#  define MCP_CANINTE_WAKIE 0x40
#  define MCP_CANINTE_ERRIE 0x20
#  define MCP_CANINTE_TX2IE 0x10
#  define MCP_CANINTE_TX1IE 0x08
#  define MCP_CANINTE_TX0IE 0x04
#  define MCP_CANINTE_RX1IE 0x02
#  define MCP_CANINTE_RX0IE 0x01

#define MCP_EFLG                0x2d
#   define MCP_EFLG_EWARN       0x01
#   define MCP_EFLG_RXWAR       0x02
#   define MCP_EFLG_TXWAR       0x04
#   define MCP_EFLG_RXEP        0x08
#   define MCP_EFLG_TXEP        0x10
#   define MCP_EFLG_TXBO        0x20
#   define MCP_EFLG_RX0OVR      0x40
#   define MCP_EFLG_RX1OVR      0x80
#   define MCP_EFLG_ERRORMASK   0xF8

#define MCP_CNF1          0x2a
#  define MCP_CNF1_SJW_SHIFT   6
#define MCP_CNF2          0x29
#  define MCP_CNF2_BTLMODE     0x80
#  define MCP_CNF2_SAM         0x40
#  define MCP_CNF2_PS1_SHIFT   3
#define MCP_CNF3          0x28
#  define MCP_CNF3_SOF     0x08
#  define MCP_CNF3_WAKFIL      0x04
#  define MCP_CNF3_PHSEG2_MASK 0x07

#define MCP_STAT_RX0IF (1 << 0)
#define MCP_STAT_RX1IF (1 << 1)

#define MCP_RATE_20KBPS   1
#define MCP_RATE_125KBPS  2
/*
#define MCP_CNF_SJW1            0x00
#define MCP_CNF_SJW2            0x40
#define MCP_CNF_SJW3            0x80
#define MCP_CNF_SJW4            0xC0
#define MCP_4MHz_125kBPS_SJW    MCP_CNF_SJW1
#define MCP_4MHz_125kBPS_CFG1   (MCP_4MHz_125kBPS_SJW | 1)
*/
void mcp_init(void);
void mcp_select(void);
void mcp_unselect(void);

uint8_t mcp_read_reg(uint8_t reg);
void mcp_write_reg(uint8_t reg, uint8_t val);
void mcp_modify_reg(uint8_t reg, uint8_t mask, uint8_t val);
void mcp_reset(void);
uint8_t mcp_check_reg(uint8_t reg, uint8_t mask, uint8_t val);
uint8_t mcp_check_reset_result(void);
uint8_t mcp_probe(void);

uint8_t mcp_set_normal_mode(void);
uint8_t mcp_set_sleep_mode(void);
uint8_t mcp_set_loopback_mode(void);
uint8_t mcp_set_listen_only_mode(void);
uint8_t mcp_set_configuration_mode(void);

uint8_t mcp_get_free_txbuf(uint8_t * txbuf_n);
void mcp_read_frame(uint8_t buf_begin_addr, can_frame * msg);
void mcp_write_frame(uint8_t buf_begin_addr, const can_frame * msg);
void mcp_start_transmit(uint8_t buf_ctrl);
uint8_t mcp_read_status(void);
uint8_t mcp_config_rate(uint8_t can_speed);

