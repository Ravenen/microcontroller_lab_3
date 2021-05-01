#include "keypad.h"

const char keyboard[4][4] = { { '1', '2', '3', 'A' },
                             { '4', '5', '6', 'B' },
                             { '7', '8', '9', 'C' },
                             { '*', '0', '#', 'D' } };
const uint8_t rows[4] = { KEYBOARD_ROW_A, KEYBOARD_ROW_B, KEYBOARD_ROW_C, KEYBOARD_ROW_D };
bool is_button_down = false;

void keypadInit() {
    KEYBOARD_DDR = KEYBOARD_IO_MATRIX_MASK; // rows on output, cols on input
    KEYBOARD_PORT = ~KEYBOARD_IO_MATRIX_MASK; // resistors for output
}

char getPressedKey() {
    char key_pressed = 0;
    if (KEYBOARD_PIN != 0xF0 && !is_button_down) {
        is_button_down = true;
        int row;
        int column = findBitPosition(~(KEYBOARD_PIN >> 4) & 0b00001111);
        for (unsigned i = 0; i < (sizeof(rows) / sizeof(*rows)); i++) {
            KEYBOARD_PORT = ~(1 << rows[i]);
            if (KEYBOARD_PORT != KEYBOARD_PIN) {
                row = i;
                break;
            }
        }
        KEYBOARD_PORT = ~KEYBOARD_IO_MATRIX_MASK;
        key_pressed = keyboard[row][column];
    }
    else if (KEYBOARD_PIN == 0xF0) {
        is_button_down = false;
    }
    return key_pressed;
}

uint8_t findBitPosition(uint8_t number) {
    uint8_t i = 1, pos = 0;
    while (!(i & number)) {
        i = i << 1;
        pos++;
    }
    return pos;
}