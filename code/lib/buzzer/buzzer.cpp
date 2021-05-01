#include "buzzer.h"

void buzzerInit() {
    BUZZER_DDR |= 1 << BUZZER;
}

void playSound(SoundType sound) {
    switch (sound) {
    case CLICK:
        _delay_ms(50);
        BUZZER_PORT |= 1 << BUZZER;
        _delay_ms(50);
        BUZZER_PORT &= ~(1 << BUZZER);
        break;
    case SUCCESS:
        _delay_ms(50);
        BUZZER_PORT |= 1 << BUZZER;
        _delay_ms(50);
        BUZZER_PORT &= ~(1 << BUZZER);
        _delay_ms(50);
        BUZZER_PORT |= 1 << BUZZER;
        _delay_ms(50);
        BUZZER_PORT &= ~(1 << BUZZER);
        break;
    case ERROR:
        _delay_ms(50);
        BUZZER_PORT |= 1 << BUZZER;
        _delay_ms(200);
        BUZZER_PORT &= ~(1 << BUZZER);
        break;
    case ALARM:
        _delay_ms(50);
        BUZZER_PORT |= 1 << BUZZER;
        _delay_ms(200);
        BUZZER_PORT &= ~(1 << BUZZER);
        _delay_ms(100);
        break;
    }
}