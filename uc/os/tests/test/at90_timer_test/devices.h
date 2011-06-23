#ifndef DEVICES_H_
#define DEVICES_H_

#include <stdint.h>

void devices_init(void);
void blink(volatile uint8_t * port, uint8_t shift, unsigned int n, unsigned int interval_ms);

void led1_on(void);
void led1_off(void);
void led1_toggle(void);

void led2_on(void);
void led2_off(void);

void led3_on(void);
void led3_off(void);
void led3_toggle(void);

void led1_blink(unsigned int n, unsigned int interval_ms);
void led2_blink(unsigned int n, unsigned int interval_ms);
void led3_blink(unsigned int n, unsigned int interval_ms);

uint8_t button_state(void);


#endif /* DEVICES_H_ */
