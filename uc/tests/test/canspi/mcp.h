#define INSTRUCTION_WRITE       0x02
#define INSTRUCTION_READ        0x03
#define INSTRUCTION_BIT_MODIFY  0x05
#define INSTRUCTION_LOAD_TXB(n) (0x40 + 2 * (n))
#define INSTRUCTION_READ_RXB(n) (((n) == 0) ? 0x90 : 0x94)
#define INSTRUCTION_RESET       0xC0

#define CANSTAT                     0x0e
#define CANCTRL                     0x0f
#  define CANCTRL_REQOP_MASK          0xe0
#  define CANCTRL_REQOP_CONF          0x80
#  define CANCTRL_REQOP_LISTEN_ONLY   0x60
#  define CANCTRL_REQOP_LOOPBACK      0x40
#  define CANCTRL_REQOP_SLEEP         0x20
#  define CANCTRL_REQOP_NORMAL        0x00
#  define CANCTRL_OSM                 0x08
#  define CANCTRL_ABAT                0x10

#define CANINTF       0x2c
#  define CANINTF_MERRF 0x80
#  define CANINTF_WAKIF 0x40
#  define CANINTF_ERRIF 0x20
#  define CANINTF_TX2IF 0x10
#  define CANINTF_TX1IF 0x08
#  define CANINTF_TX0IF 0x04
#  define CANINTF_RX1IF 0x02
#  define CANINTF_RX0IF 0x01

extern const int cs_pin;
extern const int button_pin;

byte mcp_read_reg(byte reg) {
    digitalWrite(cs_pin, LOW);
    byte val = 0;

    val = SPI.transfer(INSTRUCTION_READ);
    val = SPI.transfer(reg);
    val = SPI.transfer(0);
    
    digitalWrite(cs_pin, HIGH);

    return val;
}

void mcp_write_reg(byte reg, byte val) {
    digitalWrite(cs_pin, LOW);
    SPI.transfer(INSTRUCTION_WRITE);
    SPI.transfer(reg);
    SPI.transfer(val);
    digitalWrite(cs_pin, HIGH);
}

void mcp_set_normal_mode() {
    byte cctrl = mcp_read_reg(CANCTRL);

    cctrl &= ~(1 << 7);
    cctrl &= ~(1 << 6);
    cctrl &= ~(1 << 5);

    mcp_write_reg(CANCTRL, cctrl);
}

void mcp_set_sleep_mode() {
    byte cctrl = mcp_read_reg(CANCTRL);

    cctrl &= ~(1 << 7);
    cctrl &= ~(1 << 6);
    cctrl |=  (1 << 5);

    mcp_write_reg(CANCTRL, cctrl);
}

void mcp_set_loopback_mode() {
    byte cctrl = mcp_read_reg(CANCTRL);

    cctrl &= ~(1 << 7);
    cctrl |=  (1 << 6);
    cctrl &= ~(1 << 5);

    mcp_write_reg(CANCTRL, cctrl);
}

void mcp_set_listen_only_mode() {
    byte cctrl = mcp_read_reg(CANCTRL);

    cctrl &= ~(1 << 7);
    cctrl |=  (1 << 6);
    cctrl |=  (1 << 5);

    mcp_write_reg(CANCTRL, cctrl);
}

void mcp_set_configuration_mode() {
    byte cctrl = mcp_read_reg(CANCTRL);

    cctrl |=  (1 << 7);
    cctrl &= ~(1 << 6);
    cctrl &= ~(1 << 5);

    mcp_write_reg(CANCTRL, cctrl);
}

void mcp_reset() {
    digitalWrite(cs_pin, LOW);
    SPI.transfer(INSTRUCTION_RESET);
    digitalWrite(cs_pin, HIGH);
    delay(5);
}

void mcp_check_reset_result() {
    for(int i = 0; i < 100; ++i) {
        byte r = mcp_read_reg(CANSTAT);

        Serial.print("mcp2515 CANSTAT: ");
        Serial.println(r, HEX);

        if((r & CANCTRL_REQOP_MASK) == CANCTRL_REQOP_CONF ) {
            Serial.print("mcp2515: entered in conf mode! Yeeeah!\n");
            return;
        }

        delay(1);
    }
 
    Serial.print("mcp2515: failed to enter in conf mode! :(\n");
}

void mcp_probe() {
    mcp_reset();

    mcp_check_reset_result();

    byte st1 = mcp_read_reg(CANSTAT) & 0xEE;
    byte st2 = mcp_read_reg(CANCTRL) & 0x17;
    
    Serial.print("mcp2515 probe: CANSTAT: ");
    Serial.println(st1, HEX);
    Serial.print("mcp2515 probe: CANCTRL: ");
    Serial.println(st2, HEX);
    
    if(st1 == 0x80 && st2 == 0x07) {
        Serial.print("mcp2515: OK\n");
    } else {
        Serial.print("mcp2515: FAILED\n");
    }
}

void mcp_probe_loopback() {
    mcp_set_loopback_mode();
    
    byte cstat = mcp_read_reg(CANSTAT) & CANCTRL_REQOP_MASK;
    byte cctrl = mcp_read_reg(CANCTRL) & CANCTRL_REQOP_MASK;

    Serial.print("mcp2515 probe loopback: CANSTAT: ");
    Serial.println(cstat, HEX);
    Serial.print("mcp2515 probe loopback: CANCTRL: ");
    Serial.println(cctrl, HEX);
    
    if(cstat == 0x40 && cctrl == 0x40) {
        Serial.print("mcp2515: entered in loopback mode\n");
    } else {
        Serial.print("mcp2515: failed to enter in loopback mode\n");
    }
} 

/**
 * Check CANINTF.RXnIF registers
 */
/*
bool mcp_check_msg() {
   //TODO     
}

MSG_TYPE mcp_get_msg() {
    //TODO
}

void mcp_send_msg(MSG_TYPE msg) {
    //TODO
}
*/

