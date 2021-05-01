#include "lcd.h"

const unsigned char ALARM_ICON_CODE[8] = { 0x04,0x0E,0x0E,0x0E,0x0E,0x1F,0x04,0x00 };
const unsigned char SPEAKER_ICON_CODE[8] = { 0x01,0x03,0x07,0x1F,0x1F,0x03,0x01,0x00 };

void lcdCheckBusy() {
    LCD_DATA_DDR &= ~((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7));

    LCD_CONTROL_PORT &= ~(1 << LCD_RS);
    LCD_CONTROL_PORT |= 1 << LCD_RW;

    uint8_t temp;
    do {
        LCD_CONTROL_PORT |= (1 << LCD_E);
        _delay_us(1);
        temp = LCD_DATA_PIN & (1 << LCD_D7);
        LCD_CONTROL_PORT &= ~(1 << LCD_E);
        LCD_CONTROL_PORT |= (1 << LCD_E);
        _delay_us(1);
        LCD_CONTROL_PORT &= ~(1 << LCD_E);
    } while (temp >> LCD_D7 == 1);

    LCD_CONTROL_PORT &= ~(1 << LCD_RW);

    LCD_DATA_DDR |= ((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7));
}

void lcdSend4bitNibble(uint8_t data) {
    LCD_DATA_PORT &= ~((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7));
    LCD_DATA_PORT |= data;
    LCD_CONTROL_PORT |= (1 << LCD_E);
    _delay_us(1);
    LCD_CONTROL_PORT &= ~(1 << LCD_E);
    _delay_us(1);
}

void lcdSendByNibbles(uint8_t data) {
    uint8_t highBits = data & HIGH_BITS_MASK;
    uint8_t lowBits = data << 4;
    lcdSend4bitNibble(highBits);
    lcdSend4bitNibble(lowBits);
}

void lcdSendCommand(uint8_t cmd) {
    lcdCheckBusy();
    LCD_CONTROL_PORT &= ~(1 << LCD_RS);
    LCD_CONTROL_PORT &= ~(1 << LCD_E);
    lcdSendByNibbles(cmd);
}

void lcdSendCharacter(char character) {
    lcdCheckBusy();
    LCD_CONTROL_PORT |= 1 << LCD_RS;
    LCD_CONTROL_PORT &= ~(1 << LCD_E);
    lcdSendByNibbles(character);
}

void lcdSendString(char* string) {
    for (int i = 0; string[i] != 0; i++) {
        lcdSendCharacter(string[i]);
    }
}

void lcdClear() {
    lcdSendCommand(LCD_FUNCTION_CLEAR);
}

void lcdSetXY(uint8_t x, uint8_t y) {
    lcdSendCommand(LCD_FUNCTION_SET_CURSOR | (LCD_SECOND_ROW * y + x));
}

void lcdRegisterIcon(Icons loc, const unsigned char* msg) {
    unsigned char i;
    if (loc < 8) {
        lcdSendCommand(0x40 + (loc * 8));	// Command 0x40 and onwards forces the device to point CGRAM address 
        for (i = 0;i < 8;i++)	// Write 8 byte for generation of 1 character
            lcdSendCharacter(msg[i]);
    }
    lcdSendCommand(LCD_FUNCTION_SET_CURSOR);
}

void lcdInit() {
    LCD_DATA_DDR |= ((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7));
    LCD_CONTROL_DDR |= ((1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_E));
    _delay_ms(100);
    LCD_CONTROL_PORT &= ~(1 << LCD_RS);
    LCD_CONTROL_PORT &= ~(1 << LCD_E);

    lcdSend4bitNibble(LCD_FUNCTION_RESET);
    _delay_ms(10);
    lcdSend4bitNibble(LCD_FUNCTION_RESET);
    _delay_us(200);
    lcdSend4bitNibble(LCD_FUNCTION_RESET);
    _delay_us(200);

    lcdSend4bitNibble(LCD_FUNCTION_SET_4_BIT);
    _delay_us(80);
    lcdSendCommand(LCD_FUNCTION_SET_4_BIT);
    _delay_us(80);

    lcdSendCommand(LCD_FUNCTION_DISPLAY_OFF);
    _delay_us(80);
    lcdSendCommand(LCD_FUNCTION_CLEAR);
    _delay_ms(4);
    lcdSendCommand(LCD_FUNCTION_ENTRYMODE);
    _delay_us(80);
    lcdSendCommand(LCD_FUNCTION_DISPLAY_ON);
    _delay_us(80);

    lcdRegisterIcon(ALARM_ICON, ALARM_ICON_CODE);
    lcdRegisterIcon(SPEAKER_ICON, SPEAKER_ICON_CODE);
}