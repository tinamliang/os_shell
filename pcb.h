// Name: Tina Liang, Student ID: 261024277 
// Name: Nicole Vilkoff, Student ID: 261027199

#ifndef PCB_H
#define PCB_H
#include <stdbool.h>
/*
 * Struct:  PCB 
 * --------------------
 * pid: process(task) id
 * PC: program counter, stores the index of line that the task is executing
 * start: the first line in shell memory that belongs to this task
 * end: the last line in shell memory that belongs to this task
 * job_length_score: for EXEC AGING use only, stores the job length score
 */
typedef struct
{
    bool priority;
    FILE* file;
    char* filename;
    int pid;
    int PC;
    int start;
    int end;
    int lines_in_file;
    int lines_executed;
    int total_lines;
    int job_length_score;
    int* array;
}PCB;

int generatePID();
PCB * makePCB(FILE* fp, char* filename, int start, int end, int* arr, int lines_in_file, int total_lines, int last_page_frame);
#endif