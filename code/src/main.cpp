#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <lcd.h>
#include <keypad.h>
#include <clock.h>
#include <buzzer.h>

#define TIMER_CYCLE_IN_SECONDS 1
#define PRESCALER_VALUE 1024

void handleKey(char key);
void resetInterface();

Clock clock;
bool is_alarm_activated = false;

char time_str_buffer[4];
uint8_t time_str_digit_offset;

char clock_string[8];
char alarm_string[8];


ISR(TIMER3_COMPA_vect) {
	if (++clock.time.seconds == 60) {
		clock.time.seconds = 0;
		if (++clock.time.minutes == 60) {
			clock.time.minutes = 0;
			if (++clock.time.hours == 24) {
				clock.time.hours = 0;
			}
		}
	}
	if (clock.function_mode == NONE) {
		sprintf(clock_string, "%02d:%02d:%02d", clock.time.hours, clock.time.minutes, clock.time.seconds);
		lcdSendCommand(LCD_FUNCTION_HOME);
		lcdSendString(clock_string);
	}
	if (clock.is_alarm_enabled &&
		clock.time.hours == clock.alarm.hours && clock.time.minutes == clock.alarm.minutes &&
		clock.time.seconds <= clock.alarm_duration_in_sec) {
		is_alarm_activated = true;
		lcdSetXY(15, 1);
		lcdSendCharacter(SPEAKER_ICON);
	}
	else if (is_alarm_activated) {
		is_alarm_activated = false;
		lcdSetXY(15, 1);
		lcdSendCharacter(' ');
	}
}

void init() {
	cli();
	TCCR3B = (1 << WGM32) | (1 << CS30) | (1 << CS32); // CTC mode, 1024 prescaler
	OCR3A = (TIMER_CYCLE_IN_SECONDS * F_CPU / PRESCALER_VALUE) - 1; // 15624 for 1 second
	TIMSK3 = 1 << OCIE3A; // enable interrupt on compare A
	sei();
	buzzerInit();
	lcdInit();
	keypadInit();
}

int main() {
	init();
	while (true) {
		char key_pressed = getPressedKey();
		if (key_pressed) {
			handleKey(key_pressed);
		}
		if (is_alarm_activated) {
			playSound(ALARM);
		}
	}
}

void handleKey(char key) {
	playSound(CLICK);

	if (key == 'A' && clock.function_mode == NONE) {
		clock.function_mode = SET_TIME;
		time_str_digit_offset = 0;
		lcdClear();
		lcdSendString((char*)"--:--");
		lcdSetXY(0, 1);
		lcdSendString((char*)"Set time");
		lcdSetXY(0, 0);
		lcdSendCommand(LCD_FUNCTION_SHOW_CURSOR);
	}
	if (key == 'B' && clock.function_mode == NONE) {
		clock.function_mode = SET_ALARM;
		time_str_digit_offset = 0;
		lcdClear();
		lcdSendString((char*)"--:--");
		lcdSetXY(0, 1);
		lcdSendString((char*)"Set alarm");
		lcdSetXY(0, 0);
		lcdSendCommand(LCD_FUNCTION_SHOW_CURSOR);
	}
	if (key == 'C' && clock.function_mode == NONE) {
		clock.function_mode = SHOW_ALARM;
		lcdClear();
		sprintf(alarm_string, "%02d:%02d:%02d", clock.alarm.hours, clock.alarm.minutes, clock.alarm.seconds);
		lcdSendCommand(LCD_FUNCTION_HOME);
		lcdSendString(alarm_string);
		lcdSetXY(0, 1);
		lcdSendString((char*)"Alarm");
		if (clock.is_alarm_enabled) {
			lcdSetXY(15, 0);
			lcdSendCharacter(ALARM_ICON);
		}
	}
	if (key == 'D' && clock.function_mode == SHOW_ALARM) {
		clock.toggleAlarm();
		resetInterface();
	}
	if (key >= '0' && key <= '9' && (clock.function_mode == SET_TIME || clock.function_mode == SET_ALARM)) {
		if (time_str_digit_offset < 4) {
			time_str_buffer[time_str_digit_offset] = key;
			lcdSendCharacter(key);
			time_str_digit_offset++;
			if (time_str_digit_offset == 2) {
				lcdSetXY(3, 0);
			}
		}
	}
	if ((key == '*' || key == '#') && clock.function_mode != NONE) {
		bool success;
		if (key == '#') {
			success = clock.setByFunctionMode(time_str_buffer);
			if (success) {
				playSound(SUCCESS);
			}
		}
		if (key == '*' || !success) {
			playSound(ERROR);
		}
		resetInterface();
	}
}

void resetInterface() {
	lcdSendCommand(LCD_FUNCTION_HIDE_CURSOR);
	lcdClear();
	if (clock.is_alarm_enabled) {
		lcdSetXY(15, 0);
		lcdSendCharacter(ALARM_ICON);
	}
	sprintf(clock_string, "%02d:%02d:%02d", clock.time.hours, clock.time.minutes, clock.time.seconds);
	lcdSendCommand(LCD_FUNCTION_HOME);
	lcdSendString(clock_string);
	clock.function_mode = NONE;
}