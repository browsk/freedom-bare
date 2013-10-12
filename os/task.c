/**
 * @file
 */

#include <string.h>
#include <stdint.h>
#include "task.h"
#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "core_cmFunc.h"
#include "uart.h"

#ifndef MAX_TASKS
#define MAX_TASKS 4
#endif

extern void context_switch(uint32_t *dest, uint32_t *source, uint32_t pc);

static void idle_function()
{
    for(;;) {
	    uart0_write_string("idle\r\n");
    }
}

uint32_t idle_stack[64] = {0};

struct task idle_task =
{ 
    .task_entry = idle_function, 
    .pc = (uint32_t)idle_function,
    .stack = idle_stack + 64, 
    .tag = {'I','D','L' },
    .state = READY
};

static void idle2_function()
{
    for(;;) {
	    uart0_write_string("working\r\n");
    }
}

uint32_t idle2_stack[64] = {0};

struct task idle2_task =
{ 
    .task_entry = idle2_function, 
    .pc = (uint32_t)idle2_function,
    .stack = idle2_stack + 64, 
    .tag = {'I','D','L' },
    .state = READY
};

int num_tasks = 0;
struct task * task_list[MAX_TASKS + 1] = { &idle_task, &idle2_task };

int current_task = 0;

#define MAX_TIMESLICE   1000
int current_timeslice = 0;
int started = 0;

int get_next_ready_task_index()
{
    int next_idx = current_task;
    do
    {
        next_idx = (next_idx + 1) % (MAX_TASKS + 1);

        // the idle task will always be READY
        if (task_list[next_idx]->state == READY)
            break;
    }
    while(1);

    return next_idx;
}

void os_tick()
{
    if (started && ++current_timeslice == MAX_TIMESLICE) {
        current_timeslice = 0;
	
        __disable_irq();

        int next_idx = get_next_ready_task_index();

        if (next_idx != current_task) {
            // context switch
            uint32_t *dest = task_list[current_task]->registers;
            uint32_t *source = task_list[next_idx]->registers;
            uint32_t newpc = task_list[current_task]->pc;

            current_task = next_idx;

            context_switch(dest, source, newpc);
/*
            asm volatile(
                    "mov  R0, %[dest]"      "\n\t"
                    "stm  R0!, {R1-R7}"     "\n\t"
                    "mov  R0, %[source]"    "\n\t"
                    "ldm  R0!, {R1-R7}" :: [dest] "r" (dest), [source] "r" (source)
                    );
  */
        }
        __enable_irq();
    }
}

int init_task(struct task * new_task)
{
    if (num_tasks == 0) {
        
    }

    if (num_tasks < MAX_TASKS + 1) {
        task_list[num_tasks++] = new_task;

        new_task->pc = 0;
        return 0;
    }
    return -1;
}

void start_tasks()
{
    started = 1;
    idle_function();
}


