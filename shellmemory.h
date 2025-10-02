// Name: Tina Liang, Student ID: 261024277 
// Name: Nicole Vilkoff, Student ID: 261027199

#ifndef SHELLMEMORY_H
#define SHELLMEMORY_H
#include "ready_queue.h"
void mem_init();
char *mem_get_value(char *var);
void mem_set_value(char *var, char *value);
int load_file(FILE* fp, char* fileID, int* arr, int first_two_frames, int lines_in_file);
int load_missing_page(FILE* fp, char* fileID, int* arr, int first_two_frames, int line_num, int num_of_pages, QueueNode* node);
char * mem_get_value_at_line(int index);
void mem_free_lines_between(int start, int end);
void printShellMemory();
int reset_mem();
#endif