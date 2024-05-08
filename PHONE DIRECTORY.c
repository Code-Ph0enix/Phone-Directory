#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_LEN 3
#define STRING_SIZE 50

struct record* new_record(char* last_name, char* first_name);
struct record* append_record(int index, char* last_name, char* first_name);
unsigned int find_hash(char* s);
unsigned int find_double_hash(char* s, int collisions, unsigned int hash);
void add_record(char* last_name, char* first_name);
void display_record(char* last_name, char* first_name);

struct record{
	int num;
	char first_name[STRING_SIZE];
	char last_name[STRING_SIZE];
	struct record* next;
};

int num_terms = 0;
struct record* records[LIST_LEN];

int main(){
	while(1){
    	printf("\nPhone Directory\n");
    	printf("[1] Add Record\n");
    	printf("[2] Search Record\n");
    	printf("[3] Exit\n");

    	printf("Select one of the above options: ");
    	int option;
    	scanf("%d", &option);

    	char first_name[STRING_SIZE];
    	char last_name[STRING_SIZE];
    	printf("\n");
    	switch(option) {
        	case 1:
            	printf("Enter the Person's First Name: ");
            	scanf("%s", first_name);
           	 
            	printf("Enter the Person's Last Name: ");
            	scanf("%s", last_name);

            	add_record(last_name, first_name);

            	break;

        	case 2:
            	printf("Enter Person's First Name to search for: ");
            	scanf("%s", first_name);

            	printf("Enter Person's Last Name to search for: ");
            	scanf("%s", last_name);

            	display_record(last_name, first_name);

            	break;

        	case 3:
            	exit(0);
            	break;

        	default:
            	printf("Enter a valid option.");
           	 
    	}
    	printf("\n");
	}
	return 0;
}

struct record* new_record(char* last_name, char* first_name){
	struct record* rec = (struct record*)malloc(sizeof(struct record));

	strncpy(rec->last_name, last_name, STRING_SIZE);
	strncpy(rec->first_name, first_name, STRING_SIZE);
    
	printf("Enter the Person's Number: ");
	scanf("%d", &rec->num);
    
	return rec;
}

struct record* append_record(int index, char* last_name, char* first_name){
	struct record* rec = (struct record*)malloc(sizeof(struct record));
    
	struct record* start = records[index];
	if(strcmp(start->last_name, last_name) == 0 && strcmp(start->first_name, first_name) == 0){
   	 
    	printf("Enter the Person's Number: ");
    	int num;
    	scanf("%d", &num);

    	if(num == start->num){
        	printf("\nThis record already exists.");
        	free(rec);
        	return start;
    	}

    	strncpy(rec->last_name, last_name, STRING_SIZE);
    	strncpy(rec->first_name, first_name, STRING_SIZE);
    	rec->num = num;
   	 
    	rec->next = start;
    	records[index] = rec;

    	printf("\nRecord added Successfully.");
    	return records[index];
	}

	rec = new_record(last_name, first_name);
	struct record* temp = start;
	while(temp->next != NULL){
    	temp = temp->next;
	}
	temp->next = rec;

	return start;
}

unsigned int find_hash(char* s) {
	unsigned int hash = 0;
	while (*s) {
    	hash = (hash * LIST_LEN) + *s;
    	s++;
	}
	return hash % LIST_LEN;
}

unsigned int find_double_hash(char* s, int collisions, unsigned int hash) {
	return (hash + collisions * (1 + (hash % (LIST_LEN - 1))) + collisions) % LIST_LEN;
}

void add_record(char* last_name, char* first_name){
	int index = find_hash(last_name);
	int collisions = 1;

	if(num_terms == LIST_LEN){
    	int original_index = index;
    	while(strcmp(records[index]->last_name, last_name) != 0){
        	if(collisions == LIST_LEN){
            	records[index] = append_record(original_index, last_name, first_name);
            	return;
        	}
        	index = find_double_hash(last_name, collisions, index) % LIST_LEN;
        	++collisions;
    	}
    	records[index] = append_record(index, last_name, first_name);
    	return;
	}

	while(records[index] != NULL){
    	if(strcmp(records[index]->last_name, last_name) == 0){
        	records[index] = append_record(index, last_name, first_name);
        	return;
    	}
    	index = find_double_hash(last_name, collisions, index) % LIST_LEN;
    	++collisions;
	}

	struct record* rec = new_record(last_name, first_name);
	records[index] = rec;
	printf("\nRecord added Successfully.");
	num_terms++;
}

int search_list(struct record* temp, char* last_name, char* first_name){
	int found = 0;
	while(temp){
    	if(strcmp(temp->last_name, last_name) == 0 && strcmp(temp->first_name, first_name) == 0){
        	printf("\n%d", temp->num);
        	found = 1;
    	}
    	temp = temp->next;
	}
	return found;
}

void display_record(char* last_name, char* first_name){
	int index = find_hash(last_name);
	int collisions = 1;
	int found = 0;

	if(LIST_LEN == num_terms){
    	int original_index = index;
    	while(strcmp(records[index]->last_name, last_name) != 0){
        	if(collisions == LIST_LEN){
            	found = search_list(records[original_index], last_name, first_name);
            	if(!found)
                	printf("\nNo matching records found for this name.");
            	return;
        	}
        	index = find_double_hash(last_name, collisions, index) % LIST_LEN;
        	++collisions;
    	}
    	int secondary_found = search_list(records[index], last_name, first_name);
    	found = (found || secondary_found);
	}

	while(records[index] != NULL){
    	if(strcmp(records[index]->last_name, last_name) == 0){
        	found = search_list(records[index], last_name, first_name);
        	return;
    	}
    	index = find_double_hash(last_name, collisions, index) % LIST_LEN;
    	++collisions;
	}
    
	printf("\nNo matching records found for this name.");
}

