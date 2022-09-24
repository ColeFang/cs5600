#include <stdlib.h>
#include "answer.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

//global value
char *mMapped
;
long length;

ElfData getELFData(const char* executable) {
    //initial data
    ElfData elfData;
    bzero(&elfData, sizeof(ElfData));

    //load the file
    FILE *fp;
    fp = open(executable, O_RDWR);
    if(fp == -1) {
        printf("fail to open the file");
        return elfData;
    }

    //gain the size of the file
    length = lseek(fp,0,SEEK_END);

    //map to memory
    if((mMapped
    = mmap(NULL, length, PROT_READ,
                       MAP_SHARED, fp, 0)) == (void*)-1)  perror("mmap\n");
    close(fp);

    // analyse ELF head
    Elf64_Ehdr *elf_head;
    elf_head = mMapped
            ;

    if (elf_head->e_ident[0] != 0x7F ||
        elf_head->e_ident[1] != 'E' ||
        elf_head->e_ident[2] != 'L' ||
        elf_head->e_ident[3] != 'F')
    {
        printf("Not a ELF file\n");
        return elfData;
    }
    elfData.elfHeader = *elf_head;
    
    //gain program headers
    Elf64_Phdr *pro_head;
    pro_head = mMapped
            +elf_head->e_phoff;
    elfData.programHeader = pro_head;
    

    // load section headers
    Elf64_Shdr *sec_head;
    sec_head = mMapped
            +elf_head->e_shoff;
    
    //load section header string table to get name.
    char *shStrTab;
    shStrTab = mMapped
            + sec_head[elf_head->e_shstrndx].sh_offset;
    char *temp;

    //allocate memory for sections
    ElfSection *elfSection = (ElfSection *)malloc(sizeof(ElfSection) * elf_head->e_shnum);
    
    //initial symbol list
    ElfSymbolList dynList;
    ElfSymbolList symList;
    dynList.size=0;
    symList.size=0;
    
    //initial pointers
    char *symStrTab;
    char *dynSymStrTab;
    Elf64_Sym *symTab;
    Elf64_Sym *dynSym;

    // load data and name for all sections
    for (int i = 0; i < elf_head->e_shnum; i++)
    {
        temp = shStrTab;
        temp = temp + sec_head[i].sh_name;
        elfSection[i].sectionName = temp;
        elfSection[i].sectionHeader = sec_head[i];
        
        //prepare for reading symbols
        if (sec_head[i].sh_type == 2){
            symTab = mMapped
                    + sec_head[i].sh_offset;
            symList.size = sec_head[i].sh_size / sizeof(Elf64_Sym);
        }
        else if (sec_head[i].sh_type == 11){
            dynSym = mMapped
                    + sec_head[i].sh_offset;
            dynList.size = sec_head[i].sh_size / sizeof(Elf64_Sym);
        }
        else if (strstr(temp,".dynstr")){
             dynSymStrTab = mMapped
                     + sec_head[i].sh_offset;
        }
        else if (strstr(temp,".strtab")){
            symStrTab = mMapped
                    + sec_head[i].sh_offset;
        }
    }
    elfData.sections = elfSection;

    //get dynamic symbol list
    ElfSymbol *dynSymbol = (ElfSymbol *) malloc(sizeof(ElfSection) * dynList.size);
    for (int i = 0; i < dynList.size; i++)
    {
        temp = dynSymStrTab + dynSym[i].st_name;
        if(*temp == 0)temp = NULL;
        dynSymbol[i].name = temp;
        dynSymbol[i].symbol = dynSym[i];
    }
    dynList.list = dynSymbol;
    elfData.dynSymbols = dynList;
    
    //get total symbol list
    ElfSymbol *symbol = (ElfSymbol *) malloc(sizeof(ElfSection) * symList.size);
    for (int i = 0; i < symList.size; i++)
    {
        temp = symStrTab + symTab[i].st_name;
        if(*temp == 0)temp = NULL;
        symbol[i].name = temp;
        symbol[i].symbol = symTab[i];
    }
    symList.list = symbol;
    elfData.otherSymbols = symList;

    //return
    return elfData;

    
}
void destroyELFData(ElfData elfData) {
    // This demonstrates valgrind leak detection.
    char* leak = malloc(1);
    
    //free pointers
    free(leak);
    free(elfData.sections);
    free(elfData.dynSymbols.list);
    free(elfData.otherSymbols.list);
    
    //unmap the memory
    if((munmap((void *)mMapped
               , length)) == -1)  perror("munmap\n");
}

