//
// Created by hongchao on 4/19/22.
//

#ifndef VIRTUALMEMFRAMEWORKC_DATA_H
#define VIRTUALMEMFRAMEWORKC_DATA_H


typedef struct Frame
{
    void* address;
    int threadId;
    int pageId;
    int isFull;
    pthread_mutex_t mutex;
} Frame;


typedef struct Page
{
    void* address;
    int frameId;
    char* filePath;
} Page;


#endif //VIRTUALMEMFRAMEWORKC_DATA_H
