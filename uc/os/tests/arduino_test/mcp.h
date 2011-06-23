#define INSTRUCTION_WRITE       0x02
#define INSTRUCTION_READ        0x03
#define INSTRUCTION_BIT_MODIFY  0x05
#define INSTRUCTION_LOAD_TXB(n) (0x40 + 2 * (n))
#define INSTRUCTION_READ_RXB(n) (((n) == 0) ? 0x90 : 0x94)
#define INSTRUCTION_RESET       0xC0

#define CANSTAT       0x0e
#define CANCTRL       0x0f
#  define CANCTRL_REQOP_MASK        0xe0
#  define CANCTRL_REQOP_CONF        0x80
#  define CANCTRL_REQOP_LISTEN_ONLY 0x60
#  define CANCTRL_REQOP_LOOPBACK    0x40
#  define CANCTRL_REQOP_SLEEP       0x20
#  define CANCTRL_REQOP_NORMAL      0x00
#  define CANCTRL_OSM               0x08
#  define CANCTRL_ABAT              0x10

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

		if((r & CANCTRL_REQOP_MASK) == CANCTRL_REQOP_CONF) {
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

