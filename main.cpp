#include <windows.h>
#include <iostream>
#include <cstring>
#include "usb_raw_access.h"
#include "filesystem.h"

using namespace std;

void interactive_cli(HANDLE);

int main(){
    HANDLE hDevice = open_device('E');
    if(hDevice == INVALID_HANDLE_VALUE){
        cout<<"Failed to open device"<<endl;
        return 1;
    }
    cout<<"Pend Drive mounted successfully and superblock loaded"<<endl;
    interactive_cli(hDevice);
    CloseHandle(hDevice);
    return 0;
}

// int main() {
//     char driveLetter='E';
//     HANDLE hDevice=open_device(driveLetter);
//     if(hDevice==INVALID_HANDLE_VALUE) {
//         cout<<"Unable to open device."<<endl;
//         return 1;
//     }

//     const int BLOCK_SIZE=512;

//     Superblock sb;
//     initialize_superblock(sb);

//     char buffer[BLOCK_SIZE]={0};
//     memcpy(buffer,&sb,sizeof(Superblock));

//     if(!write_block(hDevice,0,buffer,BLOCK_SIZE)) {
//         cout<<"Failed to write superblock."<<endl;
//         CloseHandle(hDevice);
//         return 1;
//     }

//     if(!load_bitmap(hDevice)) {
//         cout<<"Failed to load bitmap."<<endl;
//     }

//     if(!load_file_table(hDevice)) {
//         cout<<"Failed to load file table."<<endl;
//     }

//     char filename[64];
//     int size_in_blocks;

//     cout<<"Enter filename: ";
//     cin.getline(filename, sizeof(filename));

//     cout<<"Enter file size in blocks: ";
//     cin>>size_in_blocks;
//     cin.ignore();

//     if(size_in_blocks<=0) {
//         cout<<"Invalid size."<<endl;
//         CloseHandle(hDevice);
//         return 1;
//     }

//     if(create_file(hDevice,filename,size_in_blocks)) {
//         cout<<"File "<<filename<<" created successfully with "<<size_in_blocks<<" blocks."<<endl;
//     } else {
//         cout<<"Failed to create file "<<filename<<"."<<endl;
//     }

//     Superblock sb_read;
//     char readbuf[BLOCK_SIZE]={0};
//     if(!read_block(hDevice,0,readbuf,BLOCK_SIZE)) {
//         cout<<"Failed to read superblock."<<endl;
//         CloseHandle(hDevice);
//         return 1;
//     }
//     memcpy(&sb_read,readbuf,sizeof(Superblock));

//     cout<<"File system version: "<<sb_read.version<<endl;
//     cout<<"Total blocks: "<<sb_read.total_blocks<<endl;
//     cout<<"Block size: "<<sb_read.block_size<<endl;

//     CloseHandle(hDevice);
//     return 0;
// }
