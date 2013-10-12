/**
 * @file 
 *
 * Task definitions for basic OS running on the freedom board
 */

#if !defined(_TASK_H)
#define TASK_H


struct task {
    /**
     * function called when the task is started
     */
    void (*task_entry)();

    /**
     * 3 character tag used to describe the task
     */
    char tag[3];

    /**
     * pointer to the stack for this task
     */
    uint32_t *stack;

    /**
     * Task program counter
     */
    uint32_t pc;

    /**
     * 
     */
    uint32_t registers[16];

    /** 
     * The task state
     */
    enum {
        READY,         //!< Task is ready to be run
        RUNNING,       //!< Task is the currently running task
        BLOCKED        //!< Task is blocked and can't run
    } state;
};

/**
 * Initialise the task. This must be called in order to add the task to the
 * list of the tasks to be run. The user must supply a task structure with the
 * entry_function, tag and stack specified
 */
int init_task(struct task * new_task);
#endif
