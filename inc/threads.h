#ifndef __THREADS_H__
#define __THREADS_H__

#include "stdint.h"
#include "lock.h"

#define NUMBER_OF_THREADS 100
#define THREAD_STACK_SIZE 0x1000

typedef enum {
    CREATE,
    RUN,
    FINISH
} thread_state;

struct thread_struct {
	thread_state state;
	void *stack_start;
	void *stack_pointer;
};

typedef void (*function_ptr)(void*);

struct thread_data_struct {
    uint64_t r15, r14, r13, r12, rbx, rbp;
    void *start_addr;
    function_ptr function;
    void *args;
};

typedef struct thread_struct thread;
typedef struct thread_data_struct thread_data;

thread* get_thread(int index);
int get_current_thread(void);
void init_threads(void);
int create_thread(function_ptr function, void *args);
void run(int index);
void free_thread(int index);
void yield(void);
void finish_thread(void);
void thread_join(int other_id);

#endif
