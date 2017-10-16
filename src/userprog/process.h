#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

#endif /* userprog/process.h */

// 20121622
#define SPLIT_SIZE 128
#define CHAR_POINTER_SIZE sizeof(char *)
#define INT_SIZE sizeof(int);
#define VOID_POINTER_SIZE sizeof(void *)
#define WORD_ALIGN(ESP, CONTROL_NUMBER, SIZE) (SIZE =  (4 - CONTROL_NUMBER % 4); *ESP -= SIZE;)

void argument_stack (char **wordSplit, int wordIndex, void **esp);
