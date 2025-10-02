// Name: Tina Liang, Student ID: 261024277 
// Name: Nicole Vilkoff, Student ID: 261027199

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "pcb.h"
#include "kernel.h"
#include "shell.h"
#include "shellmemory.h"
#include "interpreter.h"
#include "ready_queue.h"
#include "interpreter.h"

bool active = false;
bool debug = false;
bool in_background = false;

int process_initialize(char *filename, int* arr, int first_two_frames, int total_lines, int lines_in_file){
    FILE* fp;
    int* start = (int*)malloc(sizeof(int));
    int* end = (int*)malloc(sizeof(int));
    
    fp = fopen(filename, "rt");
    if(fp == NULL){
		return FILE_DOES_NOT_EXIST;
    }
    int error_code = load_file(fp, filename, arr, first_two_frames, lines_in_file);

    if(error_code != 0){
        fclose(fp);
        return FILE_ERROR;
    }

    PCB* newPCB = makePCB(fp, filename, *start,*end, arr, total_lines, lines_in_file, first_two_frames);
    QueueNode *node = malloc(sizeof(QueueNode));
    node->pcb = newPCB;

    free(start);
    free(end);

    ready_queue_add_to_tail(node);

    fclose(fp);
    return 0;
}

int shell_process_initialize(){
    //Note that "You can assume that the # option will only be used in batch mode."
    //So we know that the input is a file, we can directly load the file into ram
    int* start = (int*)malloc(sizeof(int));
    int* end = (int*)malloc(sizeof(int));
    int error_code = 0;
    error_code = load_file(stdin, "_SHELL", 0, 0, 0);
    if(error_code != 0){
        return error_code;
    }
    PCB* newPCB = makePCB(stdin,"_SHELL", *start,*end, 0 , 0, 0, 0);
    newPCB->priority = true;
    QueueNode *node = malloc(sizeof(QueueNode));
    node->pcb = newPCB;

    ready_queue_add_to_head(node);

    freopen("/dev/tty", "r", stdin);
    return 0;
}

bool execute_process(QueueNode *node, int quanta){
    char *line = NULL;
    PCB *pcb = node->pcb;

    for(int i=0; i<quanta; i++){

        // if end of file is reached
        if (pcb->total_lines == pcb->lines_executed) { 
            terminate_process(node);
            return true;
        }

        // page has to execute lines of code not yet in memory
        else if (pcb->start>pcb->end){
            // add page table entry

            pcb->end++;
            load_missing_page(pcb->file, pcb->filename, pcb->array, 1, pcb->lines_executed, pcb->lines_in_file, node);
            in_background = false;

            // process is placed at the end of the ready queue
            return false;
        }

        else {

            // next line to execute
            line = mem_get_value_at_line(pcb->array[pcb->start] + pcb->PC);

            pcb->PC++;
            pcb->lines_executed++;

            // if the page has been completely executed
            if (pcb->PC == 3) {

                // update linked list that contains most recently used frames
                Node *cur = malloc(sizeof(Node));
                cur->page_index = pcb->array[pcb->start];

                add_to_LRU_LL(cur);

                // reset program counter
                pcb->PC = 0;

                // read next page table entry
                pcb->start++;
            }
            
            in_background = true;
            if(pcb->priority) {
                pcb->priority = false;
            }
            if (strcmp(line, "none") != 0) {
                parseInput(line);
            }
            in_background = false;
        }
    }
    return false;
}

void *scheduler_FCFS(){
    QueueNode *cur;
    while(true){
        if(is_ready_empty()) {
            if(active) continue;
            else break;   
        }
        cur = ready_queue_pop_head();
        execute_process(cur, MAX_INT);
    }
    return 0;
}

void *scheduler_SJF(){
    QueueNode *cur;
    while(true){
        if(is_ready_empty()) {
            if(active) continue;
            else break;
        }
        cur = ready_queue_pop_shortest_job();
        execute_process(cur, MAX_INT);
    }
    return 0;
}

void *scheduler_AGING_alternative(){
    QueueNode *cur;
    while(true){
        if(is_ready_empty()) {
            if(active) continue;
            else break;
        }
        cur = ready_queue_pop_shortest_job();
        ready_queue_decrement_job_length_score();
        if(!execute_process(cur, 1)) {
            ready_queue_add_to_head(cur);
        }   
    }
    return 0;
}

void *scheduler_AGING(){
    QueueNode *cur;
    int shortest;
    sort_ready_queue();
    while(true){
        if(is_ready_empty()) {
            if(active) continue;
            else break;
        }
        cur = ready_queue_pop_head();
        shortest = ready_queue_get_shortest_job_score();
        if(shortest < cur->pcb->job_length_score){
            ready_queue_promote(shortest);
            ready_queue_add_to_tail(cur);
            cur = ready_queue_pop_head();
        }
        ready_queue_decrement_job_length_score();
        if(!execute_process(cur, 1)) {
            ready_queue_add_to_head(cur);
        }
    }
    return 0;
}

void *scheduler_RR(void *arg){
    int quanta = ((int *) arg)[0];
    QueueNode *cur;
    while(true){
        if(is_ready_empty()){
            if(active) continue;
            else break;
        }
        cur = ready_queue_pop_head();
        if(!execute_process(cur, quanta)) {
            ready_queue_add_to_tail(cur);
        }
    }
    return 0;
}

int schedule_by_policy(char* policy){ //, bool mt){
    if(strcmp(policy, "FCFS")!=0 && strcmp(policy, "SJF")!=0 && 
        strcmp(policy, "RR")!=0 && strcmp(policy, "AGING")!=0 && strcmp(policy, "RR30")!=0){
            return SCHEDULING_ERROR;
    }
    if(active) return 0;
    if(in_background) return 0;
    int arg[1];
    if(strcmp("FCFS",policy)==0){
        scheduler_FCFS();
    }else if(strcmp("SJF",policy)==0){
        scheduler_SJF();
    }else if(strcmp("RR",policy)==0){
        arg[0] = 2;
        scheduler_RR((void *) arg);
    }else if(strcmp("AGING",policy)==0){
        scheduler_AGING();
    }else if(strcmp("RR30", policy)==0){
        arg[0] = 30;
        scheduler_RR((void *) arg);
    }
    return 0;
}

