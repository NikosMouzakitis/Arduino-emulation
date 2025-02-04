#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <avrcontext_arduino.h>

// Define the maximum number of tasks
#define MAX_TASKS 5

// Task states
typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    INACTIVE
} task_state_t;

// Task structure
typedef struct task {
    avr_context_t ctx;
    uint8_t stack[128];
    void (*task_func)(void);
    task_state_t state;
    struct task *next; // Circular linked list
} task_t;

// External variables
extern task_t tasks[MAX_TASKS];
extern task_t *current_task;
extern uint8_t task_count;

// Function prototypes
task_t* task_create(void (*func)(void));
void task_delete(task_t *task);

#endif // TASK_MANAGER_H
