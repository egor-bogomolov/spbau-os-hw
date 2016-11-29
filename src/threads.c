#include <threads.h>
#include <alloc.h>
#include <print.h>

extern int switch_threads(void **old_sp, void *new_sp);
extern void *start_thread;
static volatile int current_thread = 1;
volatile thread threads[100];
uint8_t size = 2;

thread* get_thread(int index) {
	return (thread*)threads + index;
}

int get_current_thread(void) {
	return current_thread;
}

void init_threads(void) {
	threads[0].state = CREATE;
	threads[1].state = RUN;
}

int create_thread(function_ptr function, void *args) {
	lock();
	volatile thread *new_thread = &threads[size];
	size++;
	new_thread->stack_start = (void*) mem_alloc(THREAD_STACK_SIZE);
	new_thread->stack_pointer = (uint8_t*) new_thread->stack_start + THREAD_STACK_SIZE - sizeof(thread_data);

	thread_data *data = new_thread->stack_pointer;
	data->r12 = 0;
	data->r13 = 0;
	data->r14 = 0;
	data->r15 = 0;
	data->rbx = 0;
	data->rbp = 0;
	data->start_addr = &start_thread;
	data->function = function;
	data->args = args;
	new_thread->state = RUN;
	unlock();
	return (int)(new_thread - threads);
}

void run(int index) {
	if (current_thread == index) {
		return;
	}
	thread *next_thread = get_thread(index);
	int previous = current_thread;
	current_thread = index;
	thread *prev_thread = get_thread(previous);
	int tmp = switch_threads(&prev_thread->stack_pointer, next_thread->stack_pointer);
	free_thread(tmp);
}

void free_thread(int index) {
	volatile thread *prev_thread = get_thread(index);
	if (prev_thread->state == FINISH && index != current_thread) {
		mem_free(prev_thread->stack_pointer);
		prev_thread->state = CREATE;
	}
}

void yield(void) {
	int i = (current_thread + 1) % size;
	while (i == 0 || threads[i].state != RUN) {
	//	printf("%d\n", i);
		i = (i + 1) % size;
	}
	run(i);
}

void finish_thread(void) {
	volatile thread* cur_thread = get_thread(current_thread);
	cur_thread->state = FINISH;
	yield();
}

void thread_join(int other_id) {
	while(1) {
		lock();
		if (threads[other_id].state != FINISH) {
			unlock();
			continue;
		}
		break;
	}
	free_thread(other_id);		
}
