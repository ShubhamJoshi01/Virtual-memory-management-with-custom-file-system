#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <windows.h>

bool read_file(HANDLE hDevice,const char* filename,char* buffer,int buffer_size);
bool write_file(HANDLE hDevice,const char* filename,const char* data,int size);
void list_files();
bool delete_file(HANDLE hDevice,const char* filename);


#endif
