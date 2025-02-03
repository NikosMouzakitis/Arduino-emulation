#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include "task.h"

#define STACK_SIZE 128
#define RUNNING 1
#define READY 0

task_t *current_task_ptr=NULL;
task_t *next_task_ptr = NULL;

uint8_t current_task = 0;

void task1(void) {
    while (1) {
        digitalWrite(13, HIGH);
        _delay_ms(500);
        digitalWrite(13, LOW);
        _delay_ms(500);
    }
}

void task2(void) {
    while (1) {
        Serial.println("Hello from Task 2!");
        _delay_ms(1000);
    }
}

uint8_t stack1[STACK_SIZE];
uint8_t stack2[STACK_SIZE];

task_t tasks[2] = {
    {stack1 + STACK_SIZE - 1, task1, READY},
    {stack2 + STACK_SIZE - 1, task2, READY}
};
  
void context_switch(task_t *current, task_t *next) {
    current_task_ptr = current;
    next_task_ptr = next;

    __asm__ __volatile__ (
        "cli                 \n\t"  // Disable interrupts for atomic context switch

        // Save current task context (Registers R0-R31)
        "push r0             \n\t"
        "in r0, __SREG__     \n\t"
        "push r0             \n\t"
        "clr r1              \n\t"  // Ensure R1 remains 0 (AVR ABI requirement)
        "push r1             \n\t"
        "push r2             \n\t"
        "push r3             \n\t"
        "push r4             \n\t"
        "push r5             \n\t"
        "push r6             \n\t"
        "push r7             \n\t"
        "push r8             \n\t"
        "push r9             \n\t"
        "push r10            \n\t"
        "push r11            \n\t"
        "push r12            \n\t"
        "push r13            \n\t"
        "push r14            \n\t"
        "push r15            \n\t"
        "push r16            \n\t"
        "push r17            \n\t"
        "push r18            \n\t"
        "push r19            \n\t"
        "push r20            \n\t"
        "push r21            \n\t"
        "push r22            \n\t"
        "push r23            \n\t"
        "push r24            \n\t"
        "push r25            \n\t"
        "push r26            \n\t"
        "push r27            \n\t"
        "push r28            \n\t"
        "push r29            \n\t"
        "push r30            \n\t"
        "push r31            \n\t"

        // Save stack pointer to current task struct
        "lds r26, current_task_ptr  \n\t"  // Load &current_task->stack_ptr into X
        "lds r27, current_task_ptr+1\n\t"
        "in r0, __SP_L__  \n\t"
        "st X+, r0        \n\t"
        "in r0, __SP_H__  \n\t"
        "st X, r0         \n\t"

        // Restore next task context
        "lds r26, next_task_ptr  \n\t"
        "lds r27, next_task_ptr+1\n\t"
        "ld r0, X+         \n\t"
        "out __SP_L__, r0  \n\t"
        "ld r0, X          \n\t"
        "out __SP_H__, r0  \n\t"

        // Restore registers
        "pop r31            \n\t"
        "pop r30            \n\t"
        "pop r29            \n\t"
        "pop r28            \n\t"
        "pop r27            \n\t"
        "pop r26            \n\t"
        "pop r25            \n\t"
        "pop r24            \n\t"
        "pop r23            \n\t"
        "pop r22            \n\t"
        "pop r21            \n\t"
        "pop r20            \n\t"
        "pop r19            \n\t"
        "pop r18            \n\t"
        "pop r17            \n\t"
        "pop r16            \n\t"
        "pop r15            \n\t"
        "pop r14            \n\t"
        "pop r13            \n\t"
        "pop r12            \n\t"
        "pop r11            \n\t"
        "pop r10            \n\t"
        "pop r9             \n\t"
        "pop r8             \n\t"
        "pop r7             \n\t"
        "pop r6             \n\t"
        "pop r5             \n\t"
        "pop r4             \n\t"
        "pop r3             \n\t"
        "pop r2             \n\t"
        "pop r1             \n\t"
        "pop r0             \n\t"
        "out __SREG__, r0   \n\t"
        "pop r0             \n\t"

        "sei                \n\t"  // Enable interrupts again
        :
        :
        : "r0", "r1", "r26", "r27", "memory"
    );
}

void setup() {
    Serial.begin(9600);
    Serial.println("serial begin 9600");
    pinMode(13, OUTPUT);
}

void loop() {
    task_t *current = &tasks[current_task];
    task_t *next    = &tasks[(current_task + 1) % 2];

    Serial.print("Switching from task ");
    Serial.print((uintptr_t)current);
    Serial.print(" to ");
    Serial.println((uintptr_t)next);

    context_switch(current, next);
    current_task = (current_task + 1) % 2;
}
