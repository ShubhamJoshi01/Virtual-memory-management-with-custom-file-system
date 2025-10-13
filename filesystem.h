#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include "usb_raw_access.h"
using namespace std;

const int BLOCK_SIZE=512;
const int TOTAL_BLOCKS=1024;

struct Superblock{
    char version[8];
    int total_blocks;
    int block_size;
    int bitmap_block;
    int filetable_block;
    int data_block;
};

struct FileEntry{
    char filename[32];
    int start_block;
    int length;
    bool in_use;
};

void initialize_superblock(Superblock& sb);
bool add_file(const char* name,int start_block,int length);
void set_block_used(int block_num);
void set_block_free(int block_num);
bool is_block_used(int block_num);

int allocate_blocks(int count);
bool save_bitmap(HANDLE hDevice);
bool load_bitmap(HANDLE hDevice);
bool save_file_table(HANDLE hDevice);
bool load_file_table(HANDLE hDevice);

bool create_file(HANDLE hDevice,const char* filename,int size_in_blocks);
#endif
