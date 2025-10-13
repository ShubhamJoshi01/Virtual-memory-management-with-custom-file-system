#ifndef FILESYSTEM_RECOVERY_H
#define FILESYSTEM_RECOVERY_H

#include <windows.h>
#include "filesystem.h"

bool load_superblock(HANDLE hDevice,Superblock &sb);
bool save_superblock(HANDLE hDevice,const Superblock &sb);
bool validate_bitmap();
bool validate_file_table();
bool load_and_validate_file_system(HANDLE hDevice, Superblock &sb);

#endif
