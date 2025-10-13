#include "usb_raw_access.h"
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

HANDLE open_device(char drive_letter);
bool read_block(HANDLE hDevice,DWORD block_num,char* buffer,DWORD block_size);
bool write_block(HANDLE hDevice,DWORD block_num,const char* buffer,DWORD block_size);

HANDLE open_device(char drive_letter){
    char device_path[]="\\\\.\\X:";
    device_path[4]=drive_letter;

    HANDLE hDevice=CreateFileA(
        device_path,
        GENERIC_READ|GENERIC_WRITE,
        FILE_SHARE_READ|FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    // HANDLE hDevice=CreateFileA(
    //     "test.img",
    //     GENERIC_READ|GENERIC_WRITE,
    //     0,
    //     NULL,
    //     CREATE_ALWAYS,
    //     FILE_ATTRIBUTE_NORMAL,
    //     NULL
    // );


    if(hDevice==INVALID_HANDLE_VALUE){
        cerr<<"Failed to open device. Please run as Administrator. Error: "<<GetLastError()<<endl;
    }
    return hDevice;
}

bool read_block(HANDLE hDevice,DWORD block_num,char* buffer,DWORD block_size){
    DWORD move_high=0;
    DWORD move_low=block_num*block_size;

    DWORD result=SetFilePointer(hDevice,move_low,(LONG*)&move_high,FILE_BEGIN);
    if(result==INVALID_SET_FILE_POINTER&&GetLastError()!=NO_ERROR){
        cerr<<"Error setting file pointer: "<<GetLastError()<<endl;
        return false;
    }

    DWORD bytesRead;
    if(!ReadFile(hDevice,buffer,block_size,&bytesRead,NULL)||bytesRead!=block_size){
        cerr<<"Error reading file: "<<GetLastError()<<endl;
        return false;
    }
    return true;
}

bool write_block(HANDLE hDevice,DWORD block_num,const char* buffer,DWORD block_size){
    DWORD move_high=0;
    DWORD move_low=block_num*block_size;

    DWORD result=SetFilePointer(hDevice,move_low,(LONG*)&move_high,FILE_BEGIN);
    if(result==INVALID_SET_FILE_POINTER&&GetLastError()!=NO_ERROR){
        cerr<<"Error setting file pointer: "<<GetLastError()<<endl;
        return false;
    }

    DWORD bytesWritten;
    if(!WriteFile(hDevice,buffer,block_size,&bytesWritten,NULL)||bytesWritten!=block_size){
        cerr<<"Error writing file: "<<GetLastError()<<endl;
        return false;
    }
    return true;
}
