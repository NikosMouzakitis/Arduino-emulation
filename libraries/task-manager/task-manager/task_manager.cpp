#include "task_manager.h"

// Global variables
task_t tasks[MAX_TASKS];
task_t *current_task = NULL;
uint8_t task_count = 0;

// Task creation function
task_t* task_create(void (*func)(void)) {
    static uint8_t next_task_index = 0;
    if (next_task_index >= MAX_TASKS) {
        return NULL; // No more task slots available
    }

    task_t *new_task = &tasks[next_task_index];
    avr_getcontext(&new_task->ctx);
    avr_makecontext(&new_task->ctx, new_task->stack, sizeof(new_task->stack), NULL, func, NULL);
    new_task->task_func = func;
    new_task->state = READY;

    if (current_task == NULL) {
        current_task = new_task;
        new_task->next = new_task; // Self-referencing for a single task
    } else {
        task_t *last_task = current_task;
        while (last_task->next != current_task) {
            last_task = last_task->next;
        }
        last_task->next = new_task; // Link the new task to the end
        new_task->next = current_task; // Close the circular linked list
    }

    next_task_index++;
    task_count++;
    return new_task;
}

// Task deletion function
void task_delete(task_t *task) {
    if (!task || task_count == 0) return;

    task_t *prev = current_task;
    while (prev->next != task && prev->next != current_task) {
        prev = prev->next;
    }

    if (prev->next == task) {
        prev->next = task->next; // Remove the task from the circular linked list
        if (task == current_task) {
            current_task = task->next; // Update the current task pointer
        }
        task->state = INACTIVE; // Mark the task as inactive
        task_count--; // Decrement the task count
    }
}
