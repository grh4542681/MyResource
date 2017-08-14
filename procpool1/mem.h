#ifndef __MEM_H__ 
#define __MEM_H__

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void *Malloc(size_t size);
void *Realloc(void *p,size_t size);

void Log( char *pcFileName, int iLine, char *pcLogStr,int mode );
#endif
