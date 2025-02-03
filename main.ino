#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avrcontext_arduino.h>

#define STACK_SIZE 128

//interrupt to run each time the timer fires.
ISR(TIMER1_COMPA_vect) {
    Serial.println("fired");
}
// Declare task functions
void task1(void* param);
void task2(void* param);

// Define the task structure
typedef struct {
    avr_context_t ctx;
    uint8_t stack[STACK_SIZE];
    void (*task_func)(void);
} task_t;

// Initialize tasks
task_t tasks[2] = {
    {{0}, {0}, task1},
    {{0}, {0}, task2}
};

// Task functions
void task1(void) {
    while (1) {
        Serial.println("Hello from Task 1");
	delay(1000);
        avr_swapcontext(&tasks[0].ctx, &tasks[1].ctx);
    }
}

void task2(void) {
    while (1) {
        Serial.println("Hello from Task 2!");
        delay(1000);
        avr_swapcontext(&tasks[1].ctx, &tasks[0].ctx);
    }
}

void setup() {
    Serial.begin(9600);
   
    // Configure Timer1 for a 1ms interrupt
    TCCR1A = 0;                     // Normal mode
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);  // CTC mode, Prescaler 64
    OCR1A = 31249;                    //adjust time of interrupt. 
    TIMSK1 |= (1 << OCIE1A);         // Enable Compare A Match Interrupt
    Serial.println("issuing sei");
    sei();  // Enable global interrupts

    // Initialize task contexts
    for (int i = 0; i < 2; i++) {
        avr_getcontext(&tasks[i].ctx);
        avr_makecontext(&tasks[i].ctx, tasks[i].stack, STACK_SIZE, NULL, tasks[i].task_func, NULL);
    }

    // Start the first task
    avr_setcontext(&tasks[0].ctx);
}

void loop() {
    Serial.println("Running main task...");
    delay(1000);
}


