#include <avr/io.h>
#include <util/delay.h>

#define LCD_DATA_DDR        DDRA
#define LCD_DATA_PORT       PORTA
#define LCD_DATA_PIN        PINA
#define LCD_D4              PORTA4
#define LCD_D5              PORTA5
#define LCD_D6              PORTA6
#define LCD_D7              PORTA7
#define LCD_CONTROL_DDR     DDRB
#define LCD_CONTROL_PORT    PORTB
#define LCD_RS              PORTB0
#define LCD_RW              PORTB1
#define LCD_E               PORTB2

#define LCD_FUNCTION_CLEAR          0b00000001
#define LCD_FUNCTION_HOME           0b00000010
#define LCD_FUNCTION_ENTRYMODE      0b00000110
#define LCD_FUNCTION_DISPLAY_OFF    0b00001000
#define LCD_FUNCTION_DISPLAY_ON     0b00001100
#define LCD_FUNCTION_RESET          0b00110000
#define LCD_FUNCTION_SET_4_BIT      0b00101000
#define LCD_FUNCTION_SET_CURSOR     0b10000000
#define LCD_FUNCTION_SHOW_CURSOR    0b00001101
#define LCD_FUNCTION_HIDE_CURSOR    0b00001100

#define LCD_FIRST_ROW 0x00
#define LCD_SECOND_ROW 0x40

#define HIGH_BITS_MASK 0b11110000

enum Icons {
    ALARM_ICON,
    SPEAKER_ICON
};

void lcdCheckBusy();
void lcdSend4bitNibble(uint8_t data);
void lcdSendByNibbles(uint8_t data);
void lcdSendCommand(uint8_t cmd);
void lcdSendCharacter(char character);
void lcdSendString(char* string);
void lcdClear();
void lcdSetXY(uint8_t x, uint8_t y);
void lcdRegisterIcon(Icons loc, unsigned char* msg);
void lcdInit();