#include "file_ops.h"
#include "filesystem.h"
#include "filesystem.cpp"
#include "usb_raw_access.h"
#include <iostream>
using namespace std;

bool read_file(HANDLE hDevice,const char* filename,char* buffer,int buffer_size) {
    for(int i=0;i<128;i++){
        if(file_table[i].in_use && strcmp(file_table[i].filename,filename)==0){
            int required_size=file_table[i].length*BLOCK_SIZE;
            if(buffer_size<required_size) return false;
            for(int b=0;b<file_table[i].length;b++){
                if(!read_block(hDevice,file_table[i].start_block+b,buffer+b*BLOCK_SIZE,BLOCK_SIZE))
                    return false;
            }
            return true;
        }
    }
    return false;
}

bool write_file(HANDLE hDevice,const char* filename,const char* data,int size) {
    for(int i=0;i<128;i++){
        if(file_table[i].in_use && strcmp(file_table[i].filename,filename)==0){
            int current_blocks=file_table[i].length;
            int new_blocks=(size+(BLOCK_SIZE-1))/BLOCK_SIZE;
            int to_write=min(new_blocks,current_blocks);
            for(int b=0;b<to_write;b++){
                if(!write_block(hDevice,file_table[i].start_block+b,data+b*BLOCK_SIZE,BLOCK_SIZE))
                    return false;
            }
            if(new_blocks>current_blocks){
                int additional=new_blocks-current_blocks;
                int new_start=allocate_blocks(additional);
                if(new_start<0) return false;
                for(int b=0;b<additional;b++){
                    if(!write_block(hDevice,new_start+b,data+(current_blocks+b)*BLOCK_SIZE,BLOCK_SIZE))
                        return false;
                }
                for(int b=0;b<additional;b++) set_block_used(new_start+b);
                file_table[i].length=new_blocks;
                if(!save_bitmap(hDevice)||!save_file_table(hDevice))
                    return false;
            }
            return true;
        }
    }
    return false;
}

void list_files() {
    cout<<"Files:\n";
    for(int i=0;i<128;i++){
        if(file_table[i].in_use)
            cout<<"Name:"<<file_table[i].filename<<" Start:"<<file_table[i].start_block<<" Size blocks:"<<file_table[i].length<<endl;
    }
}

bool delete_file(HANDLE hDevice,const char* filename){
    for(int i=0;i<128;i++){
        if(file_table[i].in_use && strcmp(file_table[i].filename,filename)==0){
            for(int b=0;b<file_table[i].length;b++) set_block_free(file_table[i].start_block+b);
            file_table[i].in_use=false;
            if(!save_bitmap(hDevice) || !save_file_table(hDevice))
                return false;
            return true;
        }
    }
    return false;
}
