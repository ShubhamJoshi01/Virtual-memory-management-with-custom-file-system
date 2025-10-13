#ifndef USB_RAW_ACCESS_H
#define USB_RAW_ACCESS_H

#include <windows.h>
using namespace std;

HANDLE open_device(char drive_letter);
bool read_block(HANDLE hDevice,DWORD block_num,char* buffer,DWORD block_size);
bool write_block(HANDLE hDevice,DWORD block_num,const char* buffer,DWORD block_size);

#endif
