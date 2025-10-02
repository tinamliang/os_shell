// Name: Tina Liang, Student ID: 261024277 
// Name: Nicole Vilkoff, Student ID: 261027199

#ifndef READY_QUEUE_H
#define READY_QUEUE_H
#include "pcb.h"
#define MAX_INT 2147483646
typedef struct QueueNode{
    PCB *pcb;
    struct QueueNode *next;
} QueueNode;

typedef struct Node{
    int page_index;
    struct Node *next;
} Node;

void ready_queue_destory();
void ready_queue_add_to_tail(QueueNode *node);
void add_to_LRU_LL(Node *node);
void print_LRU_LL();
void print_ready_queue();
void terminate_process(QueueNode *node);
bool is_ready_empty();
QueueNode *ready_queue_pop_shortest_job();
void ready_queue_head_to_tail();
void ready_queue_add_to_head(QueueNode *node);
QueueNode *ready_queue_pop_head();
Node *get_head_LL();
void ready_queue_decrement_job_length_score();
void sort_ready_queue();
int ready_queue_get_shortest_job_score();
void ready_queue_promote(int score);
#endif