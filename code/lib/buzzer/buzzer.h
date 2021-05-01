#include <avr/io.h>
#include "util/delay.h"

#define BUZZER_DDR DDRC
#define BUZZER_PORT PORTC
#define BUZZER PORTC0

enum SoundType {
    CLICK,
    SUCCESS,
    ERROR,
    ALARM
};

void buzzerInit();
void playSound(SoundType sound);