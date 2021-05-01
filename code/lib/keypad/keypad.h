#include <avr/io.h>
#include "util/delay.h"

#define KEYBOARD_DDR DDRF
#define KEYBOARD_PORT PORTF
#define KEYBOARD_PIN PINF
#define KEYBOARD_ROW_A PORTF0
#define KEYBOARD_ROW_B PORTF1
#define KEYBOARD_ROW_C PORTF2
#define KEYBOARD_ROW_D PORTF3
#define KEYBOARD_COL_1 PORTF4
#define KEYBOARD_COL_2 PORTF5
#define KEYBOARD_COL_3 PORTF6
#define KEYBOARD_COL_4 PORTF7

#define KEYBOARD_IO_MATRIX_MASK 0b00001111

void keypadInit();
char getPressedKey();
uint8_t findBitPosition(uint8_t number);