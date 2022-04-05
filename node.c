#include <stdio.h>

void* data;
void* next;
void* previous;

void* getData() {
    return data;
}

void* getNext() {
    return next;
}

void* getPrevious() {
    return previous;
}

void setNext(void* n) {
    next = n;
}

void setPrevious(void* p) {
    previous = p;
}


