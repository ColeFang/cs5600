#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "utils.h"
#include "callback.h"
int currentThreadId = 1;
int clear=1;
Thread* createThread() {
    Thread* ret = malloc(sizeof(Thread));
    bzero(ret, sizeof(Thread));
    Page* table = calloc(2048,sizeof(Page));
    for (int i = 0; i < 2048; ++i) {
        Page temp;
        temp.address = 0;
        temp.frameId = -1;
        temp.filePath = NULL;
        table[i] = temp;
    }
    page_table[currentThreadId] = table;
    ret->heap_end = 1024*1024;
    ret->stack_end = 8*1024*1024;
    ret->threadId = currentThreadId;
    currentThreadId++;

    return ret;
}

void destroyThread(Thread* thread) {
    // This is line is ABSOLUTELY REQUIRED for the tests to run properly. This allows the thread to finish its work
    // DO NOT REMOVE.
    if (thread->thread)pthread_join(thread->thread, NULL);

    Page* table = page_table[thread->threadId];
    for (int i = 0; i < 2048; ++i) {
        if(table[i].filePath!=NULL){
            free(table[i].filePath);
        }
    }
    free(thread);
}
