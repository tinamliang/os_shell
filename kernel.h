// Name: Tina Liang, Student ID: 261024277 
// Name: Nicole Vilkoff, Student ID: 261027199

#ifndef KERNEL
#define KERNEL
#include "pcb.h"
int process_initialize(char *filename, int* arr, int first_two_frames, int lines_in_file, int total_lines);
int schedule_by_policy(char* policy); //, bool mt);
int shell_process_initialize();
void ready_queue_destory();
#endif