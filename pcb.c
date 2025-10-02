// Name: Tina Liang, Student ID: 261024277 
// Name: Nicole Vilkoff, Student ID: 261027199

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"

int pid_counter = 1;

int generatePID(){
    return pid_counter++;
}

//In this implementation, Pid is the same as file ID 
PCB* makePCB(FILE *fp, char* filename, int start, int end, int* arr, int total_lines, int lines_in_file, int last_page_frame){
    PCB * newPCB = malloc(sizeof(PCB));
    newPCB->pid = generatePID();
    newPCB->file = fopen(filename, "r");
    newPCB->filename = filename;
    newPCB->PC = 0; // offset from start
    newPCB->start = 0; // current page table index
    newPCB->end = last_page_frame - 1; // index of last element in the page table
    newPCB->lines_in_file = lines_in_file; // number of pages 
    newPCB->lines_executed = 0;
    newPCB->total_lines = total_lines; // total number of lines in file
    newPCB->job_length_score = 1+end-start;
    newPCB->priority = false;
    newPCB->array = arr; // page table

    return newPCB;
}