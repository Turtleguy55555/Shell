#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node {
    char* data;
    char* key;
    struct node *next;
    int size;
} node;

struct node *head = NULL;
int size = 0;
void add(char* data , char* key) {
    struct node *box = (struct node*)malloc(sizeof(struct node));
    if (head == NULL) {
        box -> data = data;
        box -> key = key;
        head = box;
        size++;
    } else {
        char* getData(char* key);

        if (key != NULL && getData(key) == NULL) {
            struct node *runner = head;
            while (runner -> next != NULL) {
                runner = runner -> next;
            }
            box -> data = strdup(data);
            box -> key = strdup(key);
            runner -> next = box;
	    box -> next = NULL;
            size++;
        } else {
            struct node *runner = head;
            while (runner != NULL) {
                if (strcmp(runner->key, key) ==0) {
                    runner -> data = data;
                    break;
                }
                runner = runner -> next;
            }
        }
    }
}
char* getData(char* key) {
    struct node *runner = head;
    if(runner != NULL) {
        while (runner != NULL) {
            if (strcmp(runner -> key, key) == 0) {
                return runner -> data;
            }
            runner = runner -> next;
        }
    }
    return NULL;
}
int delete(char* key) {
    struct node *runner = head;
    if (runner != NULL && strcmp(runner -> key, key) == 0) {
        head = runner -> next;
        size--;
        return 1;
    }
    struct node *prev = NULL;
    while (runner != NULL && strcmp(runner->key, key) != 0) {
        prev = runner;
        runner = runner -> next;
    }
    if (runner != NULL) {
        prev -> next = runner -> next;
        free(runner);
        size--;
        return 1;
    }
    return -1;
}

void freeList() {
    struct node *runner;
    while (head != NULL) {
        runner = head;
        head = head -> next;
        free(runner);
    }
}

void list() {
    
    struct node *runner = head;
    if(runner != NULL) {	
        while (runner != NULL) {
            char* s = runner -> key;
            char* d = runner -> data;
            write(1, s, strlen(s));
            write(1, " ", 1);
            write(1, d, strlen(d));
            write(1, "\n", 1);
            runner = runner -> next;
        }
    }
}







