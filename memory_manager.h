// VirtualMemoryManager.h
#ifndef VIRTUAL_MEMORY_MANAGER_H
#define VIRTUAL_MEMORY_MANAGER_H

#include <windows.h>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>

const int RAM_FRAMES = 4;     // Number of physical frames in RAM
const int PAGE_SIZE = 4096;   // Page size in bytes

struct Page {
    int page_num;
    bool valid;
    bool referenced;
    char data[PAGE_SIZE];
};

class VirtualMemoryManager {
private:
    std::vector<Page> ram;          // RAM frames
    std::queue<int> fifo_queue;     // For FIFO replacement
    std::unordered_map<int,int> page_table; // page_num -> frame index
    HANDLE hDevice;                 // Pendrive device handle (swap)

public:
    VirtualMemoryManager(HANDLE device): hDevice(device) {
        ram.resize(RAM_FRAMES);
        for(auto &p: ram) p.valid = false;
    }

    bool load_page_from_swap(int page_num, int frame);
    bool write_page_to_swap(int page_num, int frame);
    bool access_page(int page_num);
};

#endif
