// VirtualMemoryManager.cpp
#include "memory_manager.h"
#include "usb_raw_access.h"  // Your existing USB read/write functions

bool VirtualMemoryManager::load_page_from_swap(int page_num, int frame){
    return read_block(hDevice, page_num, ram[frame].data, PAGE_SIZE);
}

bool VirtualMemoryManager::write_page_to_swap(int page_num, int frame){
    return write_block(hDevice, page_num, ram[frame].data, PAGE_SIZE);
}

bool VirtualMemoryManager::access_page(int page_num){
    if(page_table.find(page_num) != page_table.end()){
        int frame = page_table[page_num];
        ram[frame].referenced = true;
        std::cout<<"Page "<<page_num<<" accessed in frame "<<frame<<std::endl;
        return true;
    }
    int frame_to_use = -1;
    for(int i=0; i < RAM_FRAMES; i++) {
        if(!ram[i].valid) {
            frame_to_use = i;
            break;
        }
    }
    if(frame_to_use == -1){
        frame_to_use = fifo_queue.front();
        fifo_queue.pop();

        write_page_to_swap(ram[frame_to_use].page_num, frame_to_use);
        page_table.erase(ram[frame_to_use].page_num);
    }
    if(!load_page_from_swap(page_num, frame_to_use)){
        std::cout<<"Failed to load page from swap"<<std::endl;
        return false;
    }
    ram[frame_to_use].valid = true;
    ram[frame_to_use].page_num = page_num;
    ram[frame_to_use].referenced = true;
    page_table[page_num] = frame_to_use;
    fifo_queue.push(frame_to_use);

    std::cout<<"Page "<<page_num<<" loaded into frame "<<frame_to_use<<std::endl;
    return true;
}
