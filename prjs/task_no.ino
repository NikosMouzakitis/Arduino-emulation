#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

// Define stack size and task states
#define STACK_SIZE 128
#define RUNNING 1
#define READY 0

// Define the task structure directly here
typedef struct {
    uint16_t *stack_ptr;
    void (*task_func)(void);
    uint8_t state;
} task_t;

// Declare global task pointers
task_t *current_task_ptr;
task_t *next_task_ptr;
// Task-related variables
uint8_t current_task = 0;

// Task functions
void task1(void) {
    while (1) {
        digitalWrite(13, HIGH);
	Serial.println("ON");
        _delay_ms(500);
        digitalWrite(13, LOW);
	Serial.println("OFF");
        _delay_ms(500);
    }
}

// Stack memory for the tasks
uint8_t stack1[STACK_SIZE];
uint8_t stack2[STACK_SIZE];

// Define the tasks array
task_t tasks[2] = {
    {(uint16_t)(stack1 + STACK_SIZE - 1), loop, RUNNING},
    {(uint16_t)(stack2 + STACK_SIZE - 1), task1, READY}
};


void context_switch(task_t *current, task_t *next);
void setup() {
    Serial.begin(9600);
    Serial.println("serial begin 9600");
    Serial.print("Stack address loop(scheduler): ");
    Serial.println((uintptr_t)&stack1[0]);
    Serial.print("Stack address for task1: ");
    Serial.println((uintptr_t)&stack2[0]);
}

void loop() {

    task_t *current = &tasks[current_task];
    task_t *next = &tasks[(current_task + 1) % 3];  // Switch 

    Serial.print("Switching from task ");
    Serial.print(current_task);
    Serial.print(" to task ");
    Serial.println((current_task+1));
    Serial.println("****");
    context_switch(current, next);	
    Serial.println("2222222222");
   
    delay(1000);
}


//  Proper context switching function
void context_switch(task_t *current, task_t *next) {
 //   current_task_ptr = current;
  //  next_task_ptr = next;

    Serial.println("se1");
    // Save registers and PC
    asm volatile(
        "push r0                \n\t"
         // Save the stack pointer
        "in r0, __SP_L__        \n\t"
        "st %a0, r0             \n\t"
        "in r0, __SP_H__        \n\t"
        "std %a0+1, r0          \n\t"
        :
        : "e" (&current->stack_ptr)
        : "r0"
    );
   
    Serial.println("se0");

}


