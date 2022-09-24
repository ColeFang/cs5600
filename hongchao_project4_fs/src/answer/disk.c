
#include <stdbool.h>
#include "../disk.h"

// The following functions are to be implemented by the students (see disk.h for documentation)
int split(char** dst, char* str, const char* spl)
{
    int n = 0;
    char *result = malloc(20);
    result = strtok(str, spl);
    while( result != NULL )
    {
        dst[n++]=result;
        result = strtok(NULL, spl);
    }
    return n;
}
void destroyWriteResponse(WriteResponse* writeResponse){
    free(writeResponse->sectors);
    free(writeResponse);
}
int permit(INode iNode, int uid, int gid, int act){
    int m=0;
    int p1=iNode.permissions/100;
    int p2=(iNode.permissions%100)/10;
    int p3=(iNode.permissions%10);

    if(p3>=act){
        m=1;
    }
    else if(p2>=act && gid==iNode.gid){
        m=1;
    }
    else if(p1>=act && uid==iNode.uid && gid==iNode.gid){
        m=1;
    }
    return m;
}
InMemoryDisk* createDisk(int size){
    InMemoryDisk* disk = malloc(sizeof(InMemoryDisk));
    int numBlock=1;
    int temp=size;
    while (temp!=1){
        temp/=2;
        numBlock*=2;
    }
    if(size>numBlock)numBlock*=2;
    numBlock/=512;
    numBlock*=2;
    char* data = ".:0\n..:0\n";
    char* memory;
    memory = malloc(numBlock*512);
    bzero(memory,numBlock*512);
    memcpy(memory,data,9);
    Bitmap freeSectorMap = createBitmap(numBlock);
    Bitmap iNodeMap = createBitmap(numBlock);
    INode* iNodeTable;
    iNodeTable = calloc(numBlock,sizeof(INode));
    iNodeTable[0].permissions=755;
    iNodeTable[0].uid=0;
    iNodeTable[0].gid=0;
    iNodeTable[0].numBlocks=1;
    iNodeTable[0].points=0;
    iNodeTable[0].bytes=9;
    freeSectorMap.bitmap[0]=1;
    iNodeMap.bitmap[0]=1;
    disk->iNodeMap=iNodeMap;
    disk->freeSectorMap=freeSectorMap;
    disk->iNodeTable=iNodeTable;
    disk->numDataBlocks=numBlock;
    disk->data= memory;
    disk->rootSector = 0;
    disk->rootINode = &iNodeTable[1];
    return disk;
}
void destroyDisk(InMemoryDisk* disk){
    free(disk->data);
    free(disk->iNodeTable);
    destroyBitmap(disk->iNodeMap);
    destroyBitmap(disk->freeSectorMap);
    free(disk);
}
WriteResponseSector createWriteResponseSector(int inode_index, BlockType bt, int index, int size){
    WriteResponseSector sector;
    sector.sector=inode_index;
    sector.blockType=bt;
    sector.chunkOrder=index;
    sector.numBytesWritten=size;
    return sector;
}
WriteResponse* writeNewFile(unsigned char *data, int size, char *fileName, char *location, InMemoryDisk *disk, int uid, int gid, int permissions){
    WriteResponse* response = malloc(sizeof(response));
    int save=size/512;
    if(size%512!=0)save+=1;
    WriteResponseSector* m;
    m= calloc(save, sizeof(WriteResponseSector));
    //WriteResponse* response= malloc(20);
    if(size>disk->numDataBlocks*512){
        return NULL;
    }
    char** dirs;
    int value=0;
    dirs= calloc(10, sizeof(char *));
    char* temp = disk->data;
    int dir_number=0;
    char* dir=malloc(50);
    char new_location[20]=".";
    if(fileName[0]=='/'){
        return NULL;
    }
    strcat(new_location,location);
    dir_number = split(dirs, new_location, "/") - 1;
    char** pairs;
    pairs = calloc(100, sizeof(char *));
    char* new_temp= malloc(50);
    if(!permit(disk->iNodeTable[value], uid, gid, 7)){
        return NULL;
    }
    for (int i = 0; i < dir_number; i++){
        strcpy(new_temp,temp);
        int key_number = split(pairs, new_temp, "\n");
        int sign=1;
        for (int j = 0; j < key_number; ++j) {
            char* key= strtok(pairs[j],":");
            value= atoi(strtok(NULL,":"));
            if(strcmp(key,dirs[i])==0){
                if(permit(disk->iNodeTable[value], uid, gid, 7)){
                    temp=disk->data+(disk->iNodeTable[value].points)*512;
                    sign=0;
                } else{
                    return NULL;
                }
            }
        }
        if(sign) return NULL;
    }
    strcpy(dir,fileName);
    free(dirs);
    free(pairs);
    free(new_temp);

    int old_index=((void*)temp-disk->data)/512;
    int inode_index = findFirstUnsetBit(disk->iNodeMap);
    strcat(dir,":");
    char str[10];
    sprintf(str,"%d", inode_index);
    strcat(dir,str);
    strcat(dir,"\n");
    memcpy(temp+disk->iNodeTable[value].bytes,dir, strlen(dir));
    disk->iNodeTable[value].bytes += strlen(dir);
    disk->iNodeTable[inode_index].permissions=permissions;
    disk->iNodeTable[inode_index].gid=gid;
    disk->iNodeTable[inode_index].uid=uid;
    disk->iNodeTable[inode_index].numBlocks=save;
    disk->iNodeTable[inode_index].bytes=size;
    int sector_index=0;
    free(dir);

    if(size<=126*512){
        int i=0;
        int* points= calloc(save, sizeof(int));
        while(size>0){
            sector_index = findFirstUnsetBit(disk->freeSectorMap);
            points[i]=sector_index;
            if(size<512){
                memcpy(disk->data+512*sector_index,data+(i)*512,size);
                m[i]=createWriteResponseSector(sector_index,DIRECT_BLOCK,i,size);
            }
            else{
                memcpy(disk->data+512*sector_index,data+(i)*512,512);
                m[i]=createWriteResponseSector(sector_index,DIRECT_BLOCK,i,512);
            }
            setBit(disk->freeSectorMap,sector_index);
            i+=1;
            size-=512;
        }
        sector_index = findFirstUnsetBit(disk->freeSectorMap);
        memcpy(disk->data+512*sector_index,points,4*save);
        disk->iNodeTable[inode_index].points=sector_index;
        setBit(disk->freeSectorMap,sector_index);
        free(points);
        setBit(disk->iNodeMap,inode_index);

    }
    else if(size<=126*512+128*512){
        int i;
        int* points= calloc(126, sizeof(int));
        for (i = 0; i < 126; ++i) {
            sector_index = findFirstUnsetBit(disk->freeSectorMap);
            points[i]=sector_index;
            memcpy(disk->data+512*sector_index,data+(i)*512,512);
            m[i]=createWriteResponseSector(sector_index,DIRECT_BLOCK,i,512);
            setBit(disk->freeSectorMap,sector_index);
            size-=512;
        }

        sector_index = findFirstUnsetBit(disk->freeSectorMap);
        memcpy(disk->data+512*sector_index,points,126*4);
        disk->iNodeTable[inode_index].points=sector_index;
        setBit(disk->freeSectorMap,sector_index);
        free(points);

        int* new_points= calloc(save, sizeof(int));
        while(size>0){
            sector_index = findFirstUnsetBit(disk->freeSectorMap);
            new_points[i-126]=sector_index;
            if(size<512){
                memcpy(disk->data+512*sector_index,data+(i)*512,size);
                m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,size);
            }
            else{
                memcpy(disk->data+512*sector_index,data+(i)*512,512);
                m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,512);
            }
            setBit(disk->freeSectorMap,sector_index);
            size-=512;
            i+=1;
        }
        sector_index = findFirstUnsetBit(disk->freeSectorMap);
        memcpy(disk->data+512*sector_index,new_points,4*(save-126));
        setBit(disk->freeSectorMap,sector_index);
        free(new_points);
        memcpy(disk->data+disk->iNodeTable[inode_index].points*512+(126)*4,&sector_index,4);
        setBit(disk->iNodeMap,inode_index);
    }
    else{
        int i;
        int* points= calloc(128, sizeof(int));
        for (i = 0; i < 126; ++i) {
            sector_index = findFirstUnsetBit(disk->freeSectorMap);
            points[i]=sector_index;
            memcpy(disk->data+512*sector_index,data+(i)*512,512);
            m[i]=createWriteResponseSector(sector_index,DIRECT_BLOCK,i,512);
            setBit(disk->freeSectorMap,sector_index);
            size-=512;
        }
        setBit(disk->iNodeMap,inode_index);
        sector_index = findFirstUnsetBit(disk->freeSectorMap);
        memcpy(disk->data+512*sector_index,points,126*4);
        disk->iNodeTable[inode_index].points=sector_index;
        setBit(disk->freeSectorMap,sector_index);
        free(points);

        int number=(save-126)/128;
        if((save-126)%128!=0)number+=1;
        int * new_points= calloc(save, sizeof(int ));
        while(size>0){
            sector_index = findFirstUnsetBit(disk->freeSectorMap);
            new_points[i-126]=sector_index;
            if(size<512){
                memcpy(disk->data+512*sector_index,data+(i)*512,size);
                m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,size);
            }
            else{
                memcpy(disk->data+512*sector_index,data+(i)*512,512);
                m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,512);
            }
            setBit(disk->freeSectorMap,sector_index);
            size-=512;
            i+=1;
        }
        int * fresh_points= calloc(number, sizeof(int ));
        for (int j = 0; j < number; ++j) {
            sector_index = findFirstUnsetBit(disk->freeSectorMap);
            fresh_points[j]=sector_index;
            if(j==number-1){
                memcpy(disk->data+512*sector_index,new_points+128*j,(save-126)*4-512*j);
            }
            else{
                memcpy(disk->data+512*sector_index,new_points+128*j,512);
            }
            setBit(disk->freeSectorMap,sector_index);
        }
        free(new_points);
        sector_index = findFirstUnsetBit(disk->freeSectorMap);
        memcpy(disk->data+512*sector_index,fresh_points+1,(number-1)*4);
        setBit(disk->freeSectorMap,sector_index);
        memcpy(disk->data+disk->iNodeTable[inode_index].points*512+(126)*4,fresh_points,4);
        memcpy(disk->data+disk->iNodeTable[inode_index].points*512+(127)*4,&sector_index,4);
        setBit(disk->iNodeMap,inode_index);
        free(fresh_points);
    }
    response->numSectors=save;
    response->sectors=m;
    return response;
}
WriteResponse* appendToFile(unsigned char *data, int size, char *fileName, char *location, InMemoryDisk *disk, int uid, int gid){
    WriteResponse* response= malloc(sizeof(WriteResponse));
    if(size>disk->numDataBlocks*512){
        return NULL;
    }
    char** dirs;
    int value=0;
    dirs= calloc(10, sizeof(char *));
    char* temp = disk->data;
    int dir_number=0;
    char new_location[20]=".";
    if(fileName[0]=='/'){
        return NULL;
    }
    strcat(new_location,location);
    dir_number = split(dirs, new_location, "/") - 1;
    char** pairs;
    pairs = calloc(100, sizeof(char *));
    char* new_temp= malloc(50);
    if(!permit(disk->iNodeTable[value], uid, gid, 7)){
        return NULL;
    }
    for (int i = 0; i < dir_number; i++){
        strcpy(new_temp,temp);
        int key_number = split(pairs, new_temp, "\n");
        int sign=1;
        for (int j = 0; j < key_number; ++j) {
            char* key= strtok(pairs[j],":");
            value= atoi(strtok(NULL,":"));
            if(strcmp(key,dirs[i])==0){
                if(permit(disk->iNodeTable[value], uid, gid, 7)){
                    temp=disk->data+(disk->iNodeTable[value].points)*512;
                    sign=0;
                } else{
                    return NULL;
                }
            }
        }
        if(sign) return NULL;
    }
    strcpy(new_temp,temp);
    int key_number = split(pairs, new_temp, "\n");
    for (int j = 0; j < key_number; ++j) {
        char* key= strtok(pairs[j],":");
        value= atoi(strtok(NULL,":"));
        if(strcmp(key,fileName)==0){
            break;
        }
    }
    if(!permit(disk->iNodeTable[value], uid, gid, 7)){
        return NULL;
    }
    free(dirs);
    free(pairs);
    free(new_temp);
    int bytes=disk->iNodeTable[value].bytes;
    int blocks=disk->iNodeTable[value].numBlocks;
    int save=(size)/512;
    if(size%512!=0)save+=1;
    WriteResponseSector* m;
    m= calloc(save, sizeof(WriteResponseSector));
    int sector_index=0;
    int all_size=(blocks+save)*512;
    if(all_size<=127*512){
        int i=0;
        int* points= calloc(save, sizeof(int));
        while(size>0){
            sector_index = findFirstUnsetBit(disk->freeSectorMap);
            points[i]=sector_index;
            if(size<512){
                memcpy(disk->data+512*sector_index,data+i*512,size);
                m[i]=createWriteResponseSector(sector_index,DIRECT_BLOCK,i,size);
            }
            else{
                memcpy(disk->data+512*sector_index,data+i*512,512);
                m[i]=createWriteResponseSector(sector_index,DIRECT_BLOCK,i,512);
            }
            setBit(disk->freeSectorMap,sector_index);
            i+=1;
            size-=512;
        }
        memcpy(disk->data+disk->iNodeTable[value].points*512+blocks*4,points,4*save);
        free(points);
    }
    else if(all_size<=127*512+128*512){
        int i=0;
        int* points= calloc(126, sizeof(int));
        if(blocks<=126){
            for (i = 0; i < 126-blocks; ++i) {
                sector_index = findFirstUnsetBit(disk->freeSectorMap);
                points[i]=sector_index;
                memcpy(disk->data+512*sector_index,data+(i)*512,512);
                m[i]=createWriteResponseSector(sector_index,DIRECT_BLOCK,i,512);
                setBit(disk->freeSectorMap,sector_index);
                size-=512;
            }
            memcpy(disk->data+disk->iNodeTable[value].points*512+blocks*4,points,i*4);
            free(points);
            char** new_points = calloc(save, sizeof(char *));
            while(size>0){
                sector_index = findFirstUnsetBit(disk->freeSectorMap);
                new_points[i-126+blocks]=sector_index;
                if(size<512){
                    memcpy(disk->data+512*sector_index,data+(i)*512,size);
                    m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,size);
                }
                else{
                    memcpy(disk->data+512*sector_index,data+(i)*512,512);
                    m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,512);
                }
                setBit(disk->freeSectorMap,sector_index);
                size-=512;
                i+=1;
            }
            sector_index = findFirstUnsetBit(disk->freeSectorMap);
            memcpy(disk->data+512*sector_index,new_points,4*(save-i));
            setBit(disk->freeSectorMap,sector_index);
            free(new_points);
            memcpy(disk->data+disk->iNodeTable[value].points*512+(126)*4,&sector_index,4);
        }
        else{
            free(points);
            int* new_points = calloc(save, sizeof(int));
            while(size>0){
                sector_index = findFirstUnsetBit(disk->freeSectorMap);
                new_points[i]=sector_index;
                if(size<512){
                    memcpy(disk->data+512*sector_index,data+(i)*512,size);
                    m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,size);
                }
                else{
                    memcpy(disk->data+512*sector_index,data+(i)*512,512);
                    m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,512);
                }
                setBit(disk->freeSectorMap,sector_index);
                size-=512;
                i+=1;
            }
            int* in_point = disk->data+disk->iNodeTable[value].points*512+126*4;
            int* write_in = disk->data + *in_point*512;
            memcpy(write_in+(blocks-126)*4,new_points,4*save);
            free(new_points);
        }
    }
    else{
        int i=0;
        int* points= calloc(126, sizeof(int));
        if(blocks<=126){
            for (i = 0; i < 126-blocks; ++i) {
                sector_index = findFirstUnsetBit(disk->freeSectorMap);
                points[i]=sector_index;
                memcpy(disk->data+512*sector_index,data+(i)*512,512);
                m[i]=createWriteResponseSector(sector_index,DIRECT_BLOCK,i,512);
                setBit(disk->freeSectorMap,sector_index);
                size-=512;
            }
            memcpy(disk->data+disk->iNodeTable[value].points*512+blocks*4,points,i*4);
            free(points);

            int number=(save-i)/128;
            if((save-i)%128!=0)number+=1;
            int* new_points= calloc(save, sizeof(int));
            while(size>0){
                sector_index = findFirstUnsetBit(disk->freeSectorMap);
                new_points[i-126+blocks]=sector_index;
                if(size<512){
                    memcpy(disk->data+512*sector_index,data+(i)*512,size);
                    m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,size);
                }
                else{
                    memcpy(disk->data+512*sector_index,data+(i)*512,512);
                    m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,512);
                }
                setBit(disk->freeSectorMap,sector_index);
                size-=512;
                i+=1;
            }
            int* fresh_points= calloc(number, sizeof(int));
            for (int j = 0; j < number; ++j) {
                sector_index = findFirstUnsetBit(disk->freeSectorMap);
                fresh_points[j]=sector_index;
                if(j=number-1){
                    memcpy(disk->data+512*sector_index,new_points+128*j,(save-i)*4-512*j);
                }
                else{
                    memcpy(disk->data+512*sector_index,new_points+128*j,512);
                }
                setBit(disk->freeSectorMap,sector_index);
            }
            free(new_points);
            sector_index = findFirstUnsetBit(disk->freeSectorMap);
            memcpy(disk->data+512*sector_index,fresh_points+1,(number-1)*4);
            memcpy(disk->data+disk->iNodeTable[value].points*512+(126)*4,fresh_points,4);
            memcpy(disk->data+disk->iNodeTable[value].points*512+(127)*4,&sector_index,4);
            setBit(disk->freeSectorMap,sector_index);
            free(fresh_points);
        }
        else{
            free(points);
            int* new_points= calloc(save, sizeof(int));
            while(size>0){
                sector_index = findFirstUnsetBit(disk->freeSectorMap);
                new_points[i]=sector_index;
                if(size<512){
                    memcpy(disk->data+512*sector_index,data+(i)*512,size);
                    m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,size);
                }
                else{
                    memcpy(disk->data+512*sector_index,data+(i)*512,512);
                    m[i]=createWriteResponseSector(sector_index,INDIRECT_BLOCK,i,512);
                }
                setBit(disk->freeSectorMap,sector_index);
                size-=512;
                i+=1;
            }

            int* in_point;
            int start=0;
            if((blocks-126)/128!=0){
                if(blocks<=126+128) {
                    start = 128 - (blocks - 126);
                    in_point = disk->data+disk->iNodeTable[value].points*512 + 126 * 4;
                    memcpy(in_point + (blocks - 126) * 4, new_points, start*4);
                } else{
                    in_point = disk->data+disk->iNodeTable[value].points*512 + 127 * 4;
                    int* temp_point= disk->data + *in_point*512;
                    in_point = disk->data + 512 * temp[(blocks-126-128)/128];
                    start = 128 - (blocks - 126)%128;
                    memcpy(in_point + ((blocks - 126)%128) * 4, new_points, start*4);
                }
            }
            int number=(save-start)/128;
            if((save-start)%128!=0)number+=1;
            int* fresh_points= calloc(number, sizeof(int));
            for (int j = 0; j < number; ++j) {
                sector_index = findFirstUnsetBit(disk->freeSectorMap);
                fresh_points[j]=sector_index;
                if(j=number-1){
                    memcpy(disk->data+512*sector_index,new_points+start+128*j,(save-start)*4-512*j);
                }
                else{
                    memcpy(disk->data+512*sector_index,new_points+start+128*j,512);
                }
                setBit(disk->freeSectorMap,sector_index);
            }
            if(blocks<=126+128){
                sector_index = findFirstUnsetBit(disk->freeSectorMap);
                int* temp=disk->data+512*sector_index;
                memcpy(disk->data+512*sector_index,fresh_points,number*4);
                setBit(disk->freeSectorMap,sector_index);
                memcpy(disk->data + 512 * disk->iNodeTable[value].points+(127)*4,temp,4);
            }
            else{
                int* data_p=disk->iNodeTable[value].points+(127)*4;
                int* in_data_point=disk->data + *data_p*512;
                memcpy(in_data_point+((blocks-126-128)/128)*4,fresh_points,number*4);
            }
            free(fresh_points);
            free(new_points);
        }
    }
    response->numSectors=save;
    response->sectors=m;
    return response;
}
WriteResponse* makeDir(char *dirName, char *location, struct InMemoryDisk* disk, int uid, int gid, int permissions){
    WriteResponse* response= malloc(sizeof(WriteResponse));
    char** dirs;
    int value=0;
    dirs= calloc(10, sizeof(char *));
    char* temp = disk->data;
    int dir_number=0;
    char* dir=malloc(50);
    char new_location[20]=".";
    if(dirName[0]=='/'){
        strcat(new_location,dirName);
        dir_number = split(dirs, new_location, "/") - 1;
    }
    else{
        strcat(new_location,location);
        dir_number = split(dirs, new_location, "/") - 1;
    }
    char** pairs;
    pairs = calloc(100, sizeof(char *));
    char* new_temp= malloc(20);
    for (int i = 0; i < dir_number; i++){
        strcpy(new_temp,temp);
        int key_number = split(pairs, new_temp, "\n");
        int sign=1;
        for (int j = 0; j < key_number; ++j) {
            char* key= strtok(pairs[j],":");
            value= atoi(strtok(NULL,":"));
            if(strcmp(key,dirs[i])==0){
                if(permit(disk->iNodeTable[value], uid, gid, 2)){
                    temp=disk->data+(disk->iNodeTable[value].points)*512;
                    sign=0;
                } else{
                    return NULL;
                }
            }
        }
        if(sign) return NULL;
    }
    free(new_temp);
    if(dirName[0]=='/'){
        dir=dirs[dir_number];
    } else{
        strcpy(dir,dirName);
    }
    free(dirs);
    free(pairs);
    int old_index=((void*)temp-disk->data)/512;
    int sector_index = findFirstUnsetBit(disk->freeSectorMap);
    int inode_index = findFirstUnsetBit(disk->iNodeMap);
    disk->iNodeTable[inode_index].permissions=permissions;
    disk->iNodeTable[inode_index].gid=gid;
    disk->iNodeTable[inode_index].uid=uid;
    disk->iNodeTable[inode_index].points=sector_index;
    disk->iNodeTable[inode_index].numBlocks=1;
    setBit(disk->iNodeMap,inode_index);
    strcat(dir,":");
    char str[10];
    sprintf(str,"%d", inode_index);
    strcat(dir,str);
    strcat(dir,"\n");
    memcpy(temp+disk->iNodeTable[value].bytes,dir, strlen(dir));
    disk->iNodeTable[value].bytes += strlen(dir);
    char* newData = disk->data+512*sector_index;
    char* mem = malloc(50);
    strcpy(mem,".:");
    strcat(mem,str);
    strcat(mem,"\n");
    strcat(mem,"..:");
    sprintf(str,"%d", value);
    strcat(mem,str);
    strcat(mem,"\n");
    memcpy(newData,mem,strlen(mem));
    disk->iNodeTable[inode_index].bytes= strlen(mem);
    free(mem);
    setBit(disk->freeSectorMap,sector_index);
    WriteResponseSector* sector;
    sector= calloc(1, sizeof(WriteResponseSector));
    sector->sector=inode_index;
    sector->blockType=DIRECTORY;
    sector->chunkOrder=old_index;
    sector->numBytesWritten=strlen(mem);
    disk->iNodeTable[inode_index].bytes = strlen(mem);
    response->numSectors=1;
    response->sectors=sector;
    return response;
}

bool readFile(const InMemoryDisk *disk, char* file, void* data){
    char** dirs= calloc(10, sizeof(char *));
    int value=0;
    char* temp = disk->data;
    int dir_number=0;
    char new_location[20]=".";
    strcat(new_location,file);
    dir_number = split(dirs, new_location, "/");
    char** pairs;
    pairs = calloc(100, sizeof(char *));
    char* new_temp= malloc(50);
    for (int i = 0; i < dir_number; i++){
        strcpy(new_temp,temp);
        int key_number = split(pairs, new_temp, "\n");
        int sign=1;
        for (int j = 0; j < key_number; ++j) {
            char* key= strtok(pairs[j],":");
            value= atoi(strtok(NULL,":"));
            if(strcmp(key,dirs[i])==0){
                temp=disk->data+(disk->iNodeTable[value].points)*512;
                sign=0;
            }
        }
    }
    free(dirs);
    free(pairs);
    free(new_temp);
    int sector_index=0;
    int size = disk->iNodeTable[value].bytes;
    int save = disk->iNodeTable[value].numBlocks;
    if(size<=126*512){
        int i=0;
        int* points;
        points=disk->data+(disk->iNodeTable[value].points)*512;
        while(size>0){
            if(size<512){
                memcpy(data+(i)*512,disk->data+512*points[i],size);
            }
            else{
                memcpy(data+(i)*512,disk->data+512*points[i],512);
            }
            i+=1;
            size-=512;
        }
    }
    else if(size<=126*512+128*512){
        int i;
        int* points;
        points=disk->data+(disk->iNodeTable[value].points)*512;
        for (i = 0; i < 126; ++i) {
            memcpy(data+(i)*512,disk->data+512*points[i],512);
            size-=512;
        }
        points=disk->data+points[126]*512;
        while(size>0){
            if(size<512){
                memcpy(data+(i)*512,disk->data+512*points[i-126],size);
            }
            else{
                memcpy(data+(i)*512,disk->data+512*points[i-126],512);
            }
            i+=1;
            size-=512;
        }
    }
    else{
        int i;
        int* points;
        points=disk->data+(disk->iNodeTable[value].points)*512;
        for (i = 0; i < 126; ++i) {
            memcpy(data+(i)*512,disk->data+512*points[i],512);
            size-=512;
        }
        int* m;
        m=disk->data+points[127]*512;
        points=disk->data+points[126]*512;
        while(i<126+128){
            memcpy(data+(i)*512,disk->data+512*points[i-126],512);
            i+=1;
            size-=512;
        }
        for (int j = 0; j < (save-126-128)/128; ++j) {
            points=disk->data+m[j]*512;
            if(j==(save-126-128)/128-1){
                int k=0;
                while(size>0){
                    if(size<512){
                        memcpy(data+(i)*512,disk->data+512*points[k],size);
                    }
                    else{
                        memcpy(data+(i)*512,disk->data+512*points[k],512);
                    }
                    i+=1;
                    k+=1;
                    size-=512;
                }
            }
            else{
                for (int k = 0; k < 128; ++k) {
                    memcpy(data+(i)*512,disk->data+512*points[k],512);
                    i+=1;
                    size-=512;
                }
            }
        }
    }
    return data;
}

