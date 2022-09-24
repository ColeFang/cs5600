#include <malloc.h>
#include <pthread.h>
#include "callback.h"
#include "data.h"
extern void* memory = NULL;
extern Frame* frame_table = NULL;
extern int frame_page_end = 0;
extern Page ** page_table = NULL;
extern pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void startupCallback() {
    memory = malloc(8*1024*1024);
    frame_table = calloc(2048,sizeof(Frame));
    page_table = calloc(500,sizeof(Page*));
    for (int i = 0; i < 2048; ++i) {
        Frame temp;
        temp.address = memory + i * 4 * 1024;
        temp.threadId = 0;
        temp.pageId = -1;
        temp.isFull = 0;
        pthread_mutex_init(&temp.mutex, NULL);
        frame_table[i] = temp;
    }
}
void shutdownCallback() {
    free(memory);
    free(frame_table);
    free(page_table);
}
