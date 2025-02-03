#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avrcontext_arduino.h>

#define STACK_SIZE 128

//used for context switching.
avr_context_t *current_ctx;
avr_context_t *next_ctx;
avr_context_t *tmp_ctx;


//interrupt to run each time the timer fires.
ISR(TIMER1_COMPA_vect) {
    Serial.println("fired");
    //context switch
    tmp_ctx = current_ctx;
    current_ctx = next_ctx;
    next_ctx = tmp_ctx;

    avr_swapcontext(next_ctx, current_ctx);
}
// Declare task functions
void task1(void);
void task2(void);

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
    }
}

void task2(void) {
    while (1) {
        Serial.println("Hello from Task 2!");
        delay(500);
    }
}

void setup() {
	//setup serial com.
    Serial.begin(9600);
   
    // Configure Timer1 for interrupts
    TCCR1A = 0;                     // Normal mode
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);  // CTC mode, Prescaler 64
    OCR1A = 31249;                    //adjust time of interrupt. 
    TIMSK1 |= (1 << OCIE1A);         // Enable Compare A Match Interrupt
    Serial.println("issuing sei");
    sei();  // Enable global interrupts

    // Initialize task contexts for the two tasks.
    for (int i = 0; i < 2; i++) {
        avr_getcontext(&tasks[i].ctx);
        avr_makecontext(&tasks[i].ctx, tasks[i].stack, STACK_SIZE, NULL, tasks[i].task_func, NULL);
    }
    //set up context pointers.
    current_ctx = &tasks[0].ctx;
    next_ctx = &tasks[1].ctx;

    // Start the first task(tasks[0])
    avr_setcontext(current_ctx);
}
//never executing the loop
void loop() {
    Serial.println("Running main task...");
    delay(1000);
}
