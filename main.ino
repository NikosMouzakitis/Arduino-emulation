#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "task_manager.h"

// Task function prototypes
void task1(void);
void task2(void);

// Interrupt Service Routine for Timer1
ISR(TIMER1_COMPA_vect) {
    task_t *prev_task = current_task;
    do {
        current_task = current_task->next; // Move to next task
    } while (current_task->state != READY); // Find a READY task
    avr_swapcontext(&prev_task->ctx, &current_task->ctx);
}
// Task1 
void task1(void) {
    while (1) {
        Serial.println("Task 1 running");
        _delay_ms(1000);
   }
}
// Task2
void task2(void) {
    while (1) {
        Serial.println("Task 2 running");
        _delay_ms(500);
    }
}
// Timer initialization
void isr_timer_init(void) {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    OCR1A = 31249;
    TIMSK1 |= (1 << OCIE1A);
}
// Setup function
void setup() {
    Serial.begin(9600);
    isr_timer_init();
    sei();

    // Create tasks
    task_create(task1);
    task_create(task2);

    if (current_task) {
        avr_setcontext(&current_task->ctx);
    }
}

// Loop function
void loop() {
    // Main loop remains empty; tasks run via context switching
}
