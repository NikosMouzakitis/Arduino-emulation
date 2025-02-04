#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avrcontext_arduino.h>
#define STACK_SIZE 128
#define NUM_TASKS 2

// Task states
typedef enum {
    READY,
    RUNNING,
    BLOCKED
} task_state_t;

// Task structure
typedef struct task {
    avr_context_t ctx;
    uint8_t stack[STACK_SIZE];
    void (*task_func)(void);
    task_state_t state;
    struct task *next; // Circular linked list
} task_t;

// Task function prototypes
void task1(void);
void task2(void);

// Task instances
task_t tasks[NUM_TASKS];
task_t *current_task;

// Interrupt Service Routine for Timer1
ISR(TIMER1_COMPA_vect) {
    task_t *prev_task = current_task;
    do {
        current_task = current_task->next; // Move to next task
    } while (current_task->state != READY); // Find a READY task
    avr_swapcontext(&prev_task->ctx, &current_task->ctx);
}

// Task functions
void task1(void) {
    while (1) {
        Serial.println("Task 1 running");
        _delay_ms(1000);
    }
}

void task2(void) {
    while (1) {
        Serial.println("Task 2 running");
        _delay_ms(500);
    }
}
void isr_timer_init(void)
{
    // Configure Timer1 for task switching
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    OCR1A = 31249; // Adjust for desired timing
    TIMSK1 |= (1 << OCIE1A);

}
void setup() {
    Serial.begin(9600);
    isr_timer_init();    
    sei(); // Enable global interrupts

    // Initialize tasks
    tasks[0].task_func = task1;
    tasks[1].task_func = task2;
    for (int i = 0; i < NUM_TASKS; i++) {
        avr_getcontext(&tasks[i].ctx);
        avr_makecontext(&tasks[i].ctx, tasks[i].stack, STACK_SIZE, NULL, tasks[i].task_func, NULL);
        tasks[i].state = READY;
        tasks[i].next = &tasks[(i + 1) % NUM_TASKS]; // Circular linked list
    }

    current_task = &tasks[0]; // Start with the first task
    avr_setcontext(&current_task->ctx);
}

void loop() {
    // Main loop should remain empty; tasks run in context switching
}

