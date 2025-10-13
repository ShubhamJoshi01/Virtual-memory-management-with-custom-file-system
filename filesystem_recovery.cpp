#include "filesystem_recovery.h"
#include <cstring>
#include <iostream>
#include "filesystem.cpp"
#include "usb_raw_access.h"
using namespace std;

bool load_superblock(HANDLE hDevice,Superblock &sb){
    char buf[BLOCK_SIZE] = {0};
    if(!read_block(hDevice,0,buf,BLOCK_SIZE)) return false;
    memcpy(&sb,buf,sizeof(Superblock));
    if(strncmp(sb.version,"v1.0",4)!=0) return false;
    if(sb.total_blocks != TOTAL_BLOCKS) return false;
    if(sb.block_size != BLOCK_SIZE) return false;
    return true;
}

bool save_superblock(HANDLE hDevice,const Superblock &sb){
    char buf[BLOCK_SIZE] = {0};
    memcpy(buf,&sb,sizeof(Superblock));
    return write_block(hDevice,0,buf,BLOCK_SIZE);
}

bool validate_bitmap(){
    for(int i=0; i<=9; i++) set_block_used(i);
    return true;
}

bool validate_file_table(){
    for(int i=0; i<128; i++){
        if(file_table[i].in_use){
            if(file_table[i].length <= 0) return false;
            if(file_table[i].start_block < 10 || file_table[i].start_block >= TOTAL_BLOCKS) return false;
        }
    }
    return true;
}

bool load_and_validate_file_system(HANDLE hDevice, Superblock &sb){
    if(!load_superblock(hDevice,sb)) return false;
    if(!load_bitmap(hDevice)) return false;
    if(!load_file_table(hDevice)) return false;
    if(!validate_bitmap()) return false;
    if(!validate_file_table()) return false;
    return true;
}