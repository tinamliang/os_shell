// Name: Tina Liang, Student ID: 261024277 
// Name: Nicole Vilkoff, Student ID: 261027199

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include "ready_queue.h" 
#include "kernel.h"

#define SHELL_MEM_LENGTH 10000


struct memory_struct{
	char *var;
	char *value;
};

const int FRAME = 3; 

//before this frame store, after is variable store
int THRESHOLD = FRAMESIZE; 

struct memory_struct shellmemory[SHELL_MEM_LENGTH];

// Helper functions
int match(char *model, char *var) {
	int i, len=strlen(var), matchCount=0;
	for(i=0;i<len;i++)
		if (*(model+i) == *(var+i)) matchCount++;
	if (matchCount == len)
		return 1;
	else
		return 0;
}

char *extract(char *model) {
	char token='=';    // look for this to find value
	char value[1000];  // stores the extract value
	int i,j, len=strlen(model);
	for(i=0;i<len && *(model+i)!=token;i++); // loop till we get there
	// extract the value
	for(i=i+1,j=0;i<len;i++,j++) value[j]=*(model+i);
	value[j]='\0';
	return strdup(value);
}


// Shell memory functions

void mem_init(){
	int i;
	for (i=0; i<SHELL_MEM_LENGTH; i++){		
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
}


// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
	int i;
	
	for (i=THRESHOLD+1; i<SHELL_MEM_LENGTH; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=THRESHOLD+1; i<SHELL_MEM_LENGTH; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	return;

}

//get value based on input key
char *mem_get_value(char *var_in) {
	int i;
	for (i=0; i<SHELL_MEM_LENGTH; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			return strdup(shellmemory[i].value);
		} 
	}
	return NULL;

}


void printShellMemory(){
	int count_empty = 0;
	for (int i = 0; i < SHELL_MEM_LENGTH; i++){
		if(strcmp(shellmemory[i].var,"none") == 0){
			count_empty++;
		}
		else{
			printf("\nline %d: key: %s\t\tvalue: %s\n", i, shellmemory[i].var, shellmemory[i].value);
		}
    }
	printf("\n\t%d lines in total, %d lines in use, %d lines free\n\n", SHELL_MEM_LENGTH, SHELL_MEM_LENGTH-count_empty, count_empty);
}


/*
 * Function:  addFileToMem 
 * 	Added in A2
 * --------------------
 * Load the source code of the file fp into the shell memory:
 * 		Loading format - var stores fileID, value stores a line
 *		Note that the first 100 lines are for set command, the rests are for run and exec command
 *
 *  pStart: This function will store the first line of the loaded file 
 * 			in shell memory in here
 *	pEnd: This function will store the last line of the loaded file 
 			in shell memory in here
 *  fileID: Input that need to provide when calling the function, 
 			stores the ID of the file
 * 
 * returns: error code, 21: no space left
 */
int load_file(FILE* fp, char* filename, int* arr, int first_two_frames, int lines_in_file)
{
	char *line;
    size_t i;
    int error_code = 0;
	bool hasSpaceLeft = false;
	bool flag = true;
	i=0;
	size_t candidate;
	size_t j;

	// load file can load one page or two pages depending on the number of lines in the file
	for (int m = 0; m < first_two_frames; m++) {

		while(flag){
			flag = false;
			for (i; i < THRESHOLD; i++){
				if(strcmp(shellmemory[i].var,"none") == 0){
					hasSpaceLeft = true;
					break;
				}
			}
			candidate = i;
			int z = i + 3;
			for(size_t y = i; y < z; y++){
				if(strcmp(shellmemory[y].var,"none") != 0){
					flag = true;
					break;
				}
			}
		}
		i = candidate;
		for (int x = 0; x < lines_in_file; x++) {
			if (arr[x] == -1) {
				arr[x] = i;
				break;
			}
		}

		//shell memory is full
		if(hasSpaceLeft == 0){
			error_code = 21;
			return error_code;
		}
		
		int k = i + 3;
		int l = 0;
		
		for (j = i; j < k; j++){
			if (feof(fp)) {
				break;
			}
			l++;

			line = calloc(1, SHELL_MEM_LENGTH);
			if (fgets(line, SHELL_MEM_LENGTH, fp) == NULL) {
				continue;
			}
			shellmemory[j].var = strdup(filename);
			shellmemory[j].value = strndup(line, strlen(line));
			free(line);
		}

		// if the file contains empty lines
		if (l == 1) {
			shellmemory[j].var = strdup(filename);
			shellmemory[j].value = "none";
			j++;
			shellmemory[j].var = strdup(filename);
			shellmemory[j].value = "none";
		}
		if (l == 2){
			shellmemory[j].var = strdup(filename);
			shellmemory[j].value = "none";
		}
		flag=true;
	}
	return error_code;
}

int load_missing_page(FILE* fp, char* filename, int* arr, int first_two_frames, int line_num, int num_of_pages, QueueNode* node)
{
	char *line;
	char *line1;
	char *line3;
    size_t i;
    int error_code = 0;
	bool hasSpaceLeftInFrameStore = false;
	bool flag = true;
	i=0;
	size_t candidate;
	size_t y;
	size_t q;
	size_t x;
	int count = 0;

	while(flag){
		flag = false;
		for (i; i < THRESHOLD; i++){
			if(strcmp(shellmemory[i].var,"none") == 0){
				hasSpaceLeftInFrameStore = true;
				break;
			}
		}
		candidate = i;
		int z = i + 3;
		for(size_t y = i; y < z; y++){
			if(strcmp(shellmemory[y].var,"none") != 0){
				hasSpaceLeftInFrameStore = false;
				flag = false;
				break;
			}
		}
	}

	// read from beginning of file
	fseek(fp, 0, SEEK_SET);

	// read from the line in the file that hasn't been executed
	for (x = count; x < line_num; x++) {
		line1 = calloc(1, SHELL_MEM_LENGTH);
		if (fgets(line1, SHELL_MEM_LENGTH, fp) == NULL) {
			continue;
		}
		free(line1);
	}

	// if frame store is full
	if (hasSpaceLeftInFrameStore == 0) {

		char* l1 = NULL;
		char* l2 = NULL;
		char* l3 = NULL;
	
		// get least recently used frame
		Node* lru_node = get_head_LL();

		l1 = shellmemory[lru_node->page_index].value;
		l2 = shellmemory[lru_node->page_index+1].value;
		l3 = shellmemory[lru_node->page_index+2].value;

		printf("Page fault! Victim page contents:\n");
		printf("%s%s%s", l1, l2, l3);
		printf("End of victim page contents.\n");
		
		// store new page into evicted page's memory in frame store
		int k = lru_node->page_index + 3;
		int l = 0;

		for (y = lru_node->page_index; y < k; y++){
			line = calloc(1, SHELL_MEM_LENGTH);
			if (fgets(line, SHELL_MEM_LENGTH, fp) == NULL) {
				continue;
			}
			shellmemory[y].var = strdup(filename);
			shellmemory[y].value = strndup(line, strlen(line));
			free(line);
			l++;
		}
		y--;
	
		if (l == 1) {
			shellmemory[y].var = strdup(filename);
			shellmemory[y].value = "none";
			y++;
			shellmemory[y].var = strdup(filename);
			shellmemory[y].value = "none";
		}
		if (l == 2){
			shellmemory[y].var = strdup(filename);
			shellmemory[y].value = "none";
		}

		// copying page table entries over 
		node->pcb->array[node->pcb->end] = lru_node->page_index;
		return error_code;
	}

	// if frame store is not full
	// place page in first existing hole within frame store
	
	i = candidate;

	for (int x = 0; x < num_of_pages; x++) {
		if (arr[x] == -1) {
			arr[x] = i;
			break;
		}
	}

	int p = i + 3;
	int m = 0;
	for (q = i; q < p; q++){
		if (feof(fp)) {
			break;
		} 

		else {
			line3 = calloc(1, SHELL_MEM_LENGTH);
			if (fgets(line3, SHELL_MEM_LENGTH, fp) == NULL) {
				continue;
			}
			shellmemory[q].var = strdup(filename);
			shellmemory[q].value = strndup(line3, strlen(line3));
			free(line3);
			m++;
		}
	}
	q--;

	if (m == 1) {
		shellmemory[q].var = strdup(filename);
		shellmemory[q].value = "none";
		q++;
		shellmemory[q].var = strdup(filename);
		shellmemory[q].value = "none";
	}
	if (m == 2){
		shellmemory[q].var = strdup(filename);
		shellmemory[q].value = "none";
	}

	return error_code;
}

char * mem_get_value_at_line(int index){
	if(index<0 || index > SHELL_MEM_LENGTH) return NULL; 
	return shellmemory[index].value;
}

void mem_free_lines_between(int start, int end){
	for (int i=start; i<=end && i<SHELL_MEM_LENGTH; i++){
		if(shellmemory[i].var != NULL){
			free(shellmemory[i].var);
		}	
		if(shellmemory[i].value != NULL){
			free(shellmemory[i].value);
		}	
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
}

int reset_mem() {
	for (int i=THRESHOLD+1; i<SHELL_MEM_LENGTH; i++){
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
	return 0;
}