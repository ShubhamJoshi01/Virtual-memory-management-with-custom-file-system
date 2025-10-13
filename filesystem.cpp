#include "filesystem.h"
#include "usb_raw_access.h"
#include <iostream>
#include <cstring>
using namespace std;

unsigned char bitmap[TOTAL_BLOCKS/8]={0};
FileEntry file_table[128]={0};

void set_block_used(int block_num){
    int idx=block_num/8;
    int pos=block_num%8;
    bitmap[idx]|=(1<<pos);
}

void set_block_free(int block_num){
    int idx=block_num/8;
    int pos=block_num%8;
    bitmap[idx]&=~(1<<pos);
}

bool is_block_used(int block_num){
    int idx=block_num/8;
    int pos=block_num%8;
    return (bitmap[idx]&(1<<pos))!=0;
}

void initialize_superblock(Superblock &sb){
    strcpy(sb.version,"v1.0");
    sb.total_blocks=TOTAL_BLOCKS;
    sb.block_size=BLOCK_SIZE;
    sb.bitmap_block=1;
    sb.filetable_block=2;
    sb.data_block=10;
}

bool add_file(const char* name,int start_block,int length){
    for(int i=0;i<128;i++){
        if(!file_table[i].in_use){
            strncpy(file_table[i].filename,name,31);
            file_table[i].filename[31]='\0';
            file_table[i].start_block=start_block;
            file_table[i].length=length;
            file_table[i].in_use=true;
            return true;
        }
    }
    return false;
}

int allocate_blocks(int count) {
    int consecutive=0;
    int start_block=-1;
    for(int b=10; b<TOTAL_BLOCKS; b++) {
        if(!is_block_used(b)) {
            if(consecutive==0) start_block=b;
            consecutive++;
            if(consecutive==count) {
                for(int i=0;i<count;i++) set_block_used(start_block+i);
                return start_block;
            }
        } else {
            consecutive=0;
        }
    }
    return -1;
}

bool save_bitmap(HANDLE hDevice) {
    char buf[BLOCK_SIZE]={0};
    memcpy(buf,bitmap,TOTAL_BLOCKS/8);
    return write_block(hDevice,1,buf,BLOCK_SIZE);
}

bool load_bitmap(HANDLE hDevice) {
    char buf[BLOCK_SIZE]={0};
    if(!read_block(hDevice,1,buf,BLOCK_SIZE)) return false;
    memcpy(bitmap,buf,TOTAL_BLOCKS/8);
    return true;
}

bool save_file_table(HANDLE hDevice) {
    int file_table_size=sizeof(FileEntry)*128;
    int blocks=(file_table_size+BLOCK_SIZE-1)/BLOCK_SIZE;
    char buf[blocks*BLOCK_SIZE];
    memset(buf,0,sizeof(buf));
    memcpy(buf,file_table,file_table_size);
    for(int i=0;i<blocks;i++)
        if(!write_block(hDevice,2+i,buf+i*BLOCK_SIZE,BLOCK_SIZE)) return false;
    return true;
}

bool load_file_table(HANDLE hDevice) {
    int file_table_size=sizeof(FileEntry)*128;
    int blocks=(file_table_size+BLOCK_SIZE-1)/BLOCK_SIZE;
    char buf[blocks*BLOCK_SIZE];
    memset(buf,0,sizeof(buf));
    for(int i=0;i<blocks;i++)
        if(!read_block(hDevice,2+i,buf+i*BLOCK_SIZE,BLOCK_SIZE)) return false;
    memcpy(file_table,buf,file_table_size);
    return true;
}

bool create_file(HANDLE hDevice,const char* filename,int size_in_blocks) {
    int start_block=allocate_blocks(size_in_blocks);
    if(start_block<0) return false;
    if(!add_file(filename,start_block,size_in_blocks)) {
        for(int i=0;i<size_in_blocks;i++) set_block_free(start_block+i);
        return false;
    }
    if(!save_bitmap(hDevice)||!save_file_table(hDevice)) return false;
    return true;
}
