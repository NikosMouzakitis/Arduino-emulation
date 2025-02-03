#include <Arduino.h>
#include <avr/interrupt.h>

void setup() {
    Serial.begin(9600);
    
    // Configure Timer1 for a 1ms interrupt
    TCCR1A = 0;                     // Normal mode
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);  // CTC mode, Prescaler 64
    OCR1A = 3*31249;                    //adjust time of interrupt. 
    TIMSK1 |= (1 << OCIE1A);         // Enable Compare A Match Interrupt
    Serial.println("issuing sei");
    sei();  // Enable global interrupts
}

//interrupt to run each time the timer fires.
ISR(TIMER1_COMPA_vect) {
    Serial.println("fired");
}



void loop() {
    Serial.println("Running main task...");


    delay(1000);
}

