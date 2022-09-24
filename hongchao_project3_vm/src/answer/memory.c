#include "thread.h"

#include <stdio.h>
#include "thread.h"
#include "callback.h"
#include <unistd.h>
#include <stdlib.h>
#include "memory.h"
#include "unity.h"
#include "utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


// 4k is the size of a page
const int PAGE_SIZE= 4*1024;
// A total of 8M exists in memory
const int ALL_MEM_SIZE = 8*1024*1024;
// USER Space starts at 1M
const int USER_BASE_ADDR = 1024*1024;
// Stack starts at 8M and goes down to 6M
const int STACK_END_ADDR = 6*1024*1024;
// There are total of 2048 pages
const int NUM_PAGES = 2*1024;

void swapToDisk(int threadId, int pageId){
    Page* table = page_table[threadId];
    Page page = table[pageId];
    Frame frame = frame_table[page.frameId];
    char* m = malloc(12);
    char address[12];
    char str2[10];
    sprintf(address, "%3d", threadId);
    sprintf(str2, "%4d", pageId);
    if (access(address, 0) == -1){
        mkdir(address, 0777);
    }
    strcat(address , "/");
    strcat(address , str2);
    strcat(address , ".txt");
    memcpy(m, address,12);
    FILE *fpWrite=fopen(m,"w");
    fwrite(frame.address,1,PAGE_SIZE,fpWrite);
    fclose(fpWrite);
    bzero(frame.address, PAGE_SIZE);
    table[pageId].filePath=m;
}
void swapToMemory(Page page, int size){
    FILE* file = fopen(page.filePath, "r+");
    fread(page.address, 1, size, file);
    if(remove(page.filePath) == 0 )
        printf("Removed %s.", page.filePath);
    else
        perror("remove");
    fclose(file);
}

int allocateHeapMem(Thread *thread, int size) {

    int temp = -1;
    int new_end = size+thread->heap_end;
    int page_end;
    int page_start;
    if(new_end<=STACK_END_ADDR) {
        page_start = thread->heap_end / PAGE_SIZE;
        if(new_end%PAGE_SIZE==0){
             page_end = new_end / PAGE_SIZE;
        } else{
            page_end = new_end / PAGE_SIZE + 1;
        }
        Page * table = page_table[thread->threadId];
        pthread_mutex_lock(&mutex);
        for (int i = page_start; i < page_end; ++i) {
            if(table[i].frameId == -1){
                int limit = (ALL_MEM_SIZE - 1024*1024) / PAGE_SIZE;
                table[i].frameId = frame_page_end % limit;
                table[i].address = frame_table[frame_page_end % limit].address;
                frame_page_end+=1;
            }
        }
        pthread_mutex_unlock(&mutex);
        temp = thread->heap_end;
        thread->heap_end = new_end;

    }

    return temp;
}

int allocateStackMem(Thread *thread, int size) {
    int new_end = thread->stack_end-size;
    int page_end;
    int page_start;
    int limit = (ALL_MEM_SIZE - 1024*1024) / PAGE_SIZE;
    if(new_end>=STACK_END_ADDR) {
        page_end = new_end / PAGE_SIZE;
        if(thread->stack_end%PAGE_SIZE==0){
            page_start = thread->stack_end / PAGE_SIZE;
        } else{
            page_start = thread->stack_end / PAGE_SIZE + 1;
        }
        Page * table = page_table[thread->threadId];
        pthread_mutex_lock(&mutex);
        for (int i = page_end; i < page_start; ++i) {
            if(table[i].frameId == -1){
                table[i].frameId = frame_page_end % limit;
                table[i].address = frame_table[frame_page_end % limit].address;
                frame_page_end+=1;
            }
        }
        pthread_mutex_unlock(&mutex);
        thread->stack_end = new_end;
        return new_end;
    }
    return -1;
}

void writeToAddr(const Thread* thread, int addr, int size, const void* data) {
    Page* table = page_table[thread->threadId];
    int start = addr;
    int end = addr+size;
    if (end>ALL_MEM_SIZE){
        kernelPanic(thread,addr);
    }
    while (start<end){
        int i = start/PAGE_SIZE;
        if (table[i].frameId==-1){
            kernelPanic(thread,addr);
        }
        int data_size;
        if(end <= (i + 1) * PAGE_SIZE){
            data_size = end - start;
        } else{
            data_size = (i + 1) * PAGE_SIZE - start;
        }

        pthread_mutex_lock(&frame_table[table[i].frameId].mutex);

        //swap original data into disk
        if(frame_table[table[i].frameId].threadId != 0){
            if(frame_table[table[i].frameId].threadId != thread->threadId || frame_table[table[i].frameId].isFull){
                swapToDisk(frame_table[table[i].frameId].threadId, frame_table[table[i].frameId].pageId);
            }
        }

        frame_table[table[i].frameId].threadId = thread->threadId;
        frame_table[table[i].frameId].pageId = i;

        //swap thread data into memory
        if(table[i].filePath){
            swapToMemory(table[i],addr % PAGE_SIZE);
            table[i].filePath = NULL;
        }

        //adding data
        memcpy (frame_table[table[i].frameId].address + start % PAGE_SIZE,data,data_size);
        data+=data_size;
        pthread_mutex_unlock(&frame_table[table[i].frameId].mutex);
        start = (i + 1) * PAGE_SIZE;
    }
}

void readFromAddr(Thread* thread, int addr, int size, void* outData) {
    Page* table = page_table[thread->threadId];
    int start = addr;
    int end = addr+size;

    while (start<end){
        int i = start/PAGE_SIZE;
        if (table[i].frameId==-1){
            kernelPanic(thread,addr);
        }
        int data_size;
        if(end <= (i + 1) * PAGE_SIZE){
            data_size = end - start;
        } else{
            data_size = (i + 1) * PAGE_SIZE - start;
        }
        pthread_mutex_lock(&frame_table[table[i].frameId].mutex);
        //read from disk
        if(table[i].filePath!=NULL){
            FILE* file = fopen(table[i].filePath, "r+");
            fseek(file, start % PAGE_SIZE, SEEK_SET);
            fread(outData, 1, data_size, file);
            fclose(file);
            outData+=data_size;
        } else{
            //adding data
            memcpy (outData, frame_table[table[i].frameId].address + start % PAGE_SIZE,data_size);
            outData+=data_size;
        }
        pthread_mutex_unlock(&frame_table[table[i].frameId].mutex);
        start = (i + 1) * PAGE_SIZE;
    }
}

char* getCacheFileName(Thread* thread, int addr) {
    int ind = addr / PAGE_SIZE;
    Page* table = page_table[thread->threadId];
    return table[ind].filePath;
}
