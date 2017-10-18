#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "devices/shutdown.h"
#include "userprog/process.h"

#define SYS_CALL_MAX 20
#define PHYS_BASE 0xc0000000
#define STACK 4

#define CALL_HALT(ARG_PTR) (call_halt());
#define CALL_WRITE(ARG_PTR) (call_write(*(int *)GET_ARGU(ARG_PTR, 0), *(void **)GET_ARGU(ARG_PTR, 1), *(unsigned *)GET_ARGU(ARG_PTR, 2)))
#define CALL_EXIT(ARG_PTR) (call_exit(*(int *)GET_ARGU(ARG_PTR, 0)))
#define CALL_EXEC(ARG_PTR) (call_exec(*(const char **)GET_ARGU(ARG_PTR, 0)))
#define CALL_WAIT(ARG_PTR) (call_wait(*(pid_t *)GET_ARGU(ARG_PTR, 0)))

typedef int pid_t;

inline int ARGU_HANDLER(int SYS_NUM);
inline int GET_ARGU(void *P, int index);
void call_halt (void);
int call_write (int fd, void *buffer, unsigned size);
pid_t call_exec (const char *cmd_line);
void call_exit (int status);
int call_wait (pid_t pid);

static void syscall_handler (struct intr_frame *);

	void
syscall_init (void) 
{
	printf("syscall_init\n");
	intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

	static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	int SYS_NUM;
	void *arg_ptr;
	printf("syscall_handler\n");
	hex_dump((uintptr_t)f->esp, (const char *)f->esp, (uintptr_t)PHYS_BASE - (uintptr_t)f->esp, true);	

	SYS_NUM = *((int *)f->esp);
	printf("SYSNUM : %d\n", SYS_NUM);
	arg_ptr = (void *)((int)f->esp + STACK + ARGU_HANDLER(SYS_NUM) * 4 * sizeof(int8_t));
	;
	if (SYS_CALL_MAX <= SYS_NUM) thread_exit();
	else {
		printf("switch\n");
		switch (SYS_NUM) {
			case SYS_HALT:
				CALL_HALT(arg_ptr);
				break;
			case SYS_EXIT:
				CALL_EXIT(arg_ptr);
				break;
			case SYS_WRITE:
				CALL_WRITE(arg_ptr);
				break;
			case SYS_EXEC:
				CALL_EXEC(arg_ptr);
			case SYS_WAIT:
				CALL_WAIT(arg_ptr);
			default:
				break;
		}
	}


	//  thread_exit ();

}


inline int ARGU_HANDLER(int SYS_NUM) { int SYS_CONTROL[20] = {0, 1, 1, 1, 2, 1, 1, 1, 3, 3, 2, 1, 1,}; return SYS_CONTROL[SYS_NUM];}

inline int GET_ARGU(void *P, int index) { return *((int *)P + index * STACK);}

void call_halt(void) {
	shutdown_power_off();
}

void call_exit (int status) {
	struct thread *now = thread_current();	
	now->exit_status = status;
	thread_exit();
}

pid_t call_exec (const char *cmd_line) {
	return process_execute(cmd_line);
}

int call_wait (pid_t pid) {
	return process_wait(pid);
}

int call_write (int fd, void *buffer, unsigned size) {

}
