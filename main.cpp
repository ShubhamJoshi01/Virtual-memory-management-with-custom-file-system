// // #include <windows.h>
// // #include <iostream>
// // #include <cstring>
// // #include "usb_raw_access.h"
// // #include "filesystem.h"

// // using namespace std;

// // void interactive_cli(HANDLE);

// // int main(){
// //     HANDLE hDevice = open_device('E');
// //     if(hDevice == INVALID_HANDLE_VALUE){
// //         cout<<"Failed to open device"<<endl;
// //         return 1;
// //     }
// //     cout<<"Pend Drive mounted successfully and superblock loaded"<<endl;
// //     interactive_cli(hDevice);
// //     CloseHandle(hDevice);
// //     return 0;
// // }

// // // int main() {
// // //     char driveLetter='E';
// // //     HANDLE hDevice=open_device(driveLetter);
// // //     if(hDevice==INVALID_HANDLE_VALUE) {
// // //         cout<<"Unable to open device."<<endl;
// // //         return 1;
// // //     }

// // //     const int BLOCK_SIZE=512;

// // //     Superblock sb;
// // //     initialize_superblock(sb);

// // //     char buffer[BLOCK_SIZE]={0};
// // //     memcpy(buffer,&sb,sizeof(Superblock));

// // //     if(!write_block(hDevice,0,buffer,BLOCK_SIZE)) {
// // //         cout<<"Failed to write superblock."<<endl;
// // //         CloseHandle(hDevice);
// // //         return 1;
// // //     }

// // //     if(!load_bitmap(hDevice)) {
// // //         cout<<"Failed to load bitmap."<<endl;
// // //     }

// // //     if(!load_file_table(hDevice)) {
// // //         cout<<"Failed to load file table."<<endl;
// // //     }

// // //     char filename[64];
// // //     int size_in_blocks;

// // //     cout<<"Enter filename: ";
// // //     cin.getline(filename, sizeof(filename));

// // //     cout<<"Enter file size in blocks: ";
// // //     cin>>size_in_blocks;
// // //     cin.ignore();

// // //     if(size_in_blocks<=0) {
// // //         cout<<"Invalid size."<<endl;
// // //         CloseHandle(hDevice);
// // //         return 1;
// // //     }

// // //     if(create_file(hDevice,filename,size_in_blocks)) {
// // //         cout<<"File "<<filename<<" created successfully with "<<size_in_blocks<<" blocks."<<endl;
// // //     } else {
// // //         cout<<"Failed to create file "<<filename<<"."<<endl;
// // //     }

// // //     Superblock sb_read;
// // //     char readbuf[BLOCK_SIZE]={0};
// // //     if(!read_block(hDevice,0,readbuf,BLOCK_SIZE)) {
// // //         cout<<"Failed to read superblock."<<endl;
// // //         CloseHandle(hDevice);
// // //         return 1;
// // //     }
// // //     memcpy(&sb_read,readbuf,sizeof(Superblock));

// // //     cout<<"File system version: "<<sb_read.version<<endl;
// // //     cout<<"Total blocks: "<<sb_read.total_blocks<<endl;
// // //     cout<<"Block size: "<<sb_read.block_size<<endl;

// // //     CloseHandle(hDevice);
// // //     return 0;
// // // }





// #include <windows.h>
// #include <iostream>
// #include "usb_raw_access.h"
// #include "filesystem.h"
// #include "filesystem_recovery.h"
// #include "file_ops.h"

// using namespace std;



// int main() {
//     HANDLE hDevice = open_device('E');
//     if (hDevice == INVALID_HANDLE_VALUE) {
//         cout << "Failed to open device" << endl;
//         return 1;
//     }

//     Superblock sb;
//     bool fs_loaded = load_and_validate_file_system(hDevice, sb);

//     if (!fs_loaded) {
//         cout << "Initializing new filesystem..." << endl;
//         initialize_superblock(sb);

//         // Initialize bitmap and file table in memory
//         memset(bitmap, 0, sizeof(bitmap));
//         for (int i = 0; i <= 9; i++) set_block_used(i);  // reserved blocks
//         for (int i = 0; i < 128; i++) {
//             file_table[i].in_use = false;
//             file_table[i].length = 0;
//             file_table[i].start_block = 0;
//             memset(file_table[i].filename, 0, sizeof(file_table[i].filename));
//         }

//         // Save all metadata to device
//         if (!save_superblock(hDevice, sb)
//          || !save_bitmap(hDevice)
//          || !save_file_table(hDevice)) {
//             cout << "Failed to save filesystem metadata during initialization." << endl;
//             CloseHandle(hDevice);
//             return 1;
//         }

//         cout << "Filesystem initialized successfully." << endl;
//     } else {
//         cout << "Filesystem loaded successfully. Version: " << sb.version << endl;
//     }

//     interactive_cli(hDevice);

//     CloseHandle(hDevice);
//     return 0;
// }






#include <windows.h>
#include <iostream>
#include <vector>
#include "usb_raw_access.h"
#include "filesystem.h"
#include "filesystem_recovery.h"
#include "file_ops.h"
#include "memory_manager.h"
#include "scheduling.h"

using namespace std;


void interactive_cli(HANDLE hDevice);

int main() {
    // Open USB device for swap and filesystem storage
    HANDLE hDevice = open_device('E'); 
    if(hDevice == INVALID_HANDLE_VALUE) {
        cout << "Failed to open device" << endl;
        return -1;
    }

    // Load or initialize filesystem
    Superblock sb;
    if(!load_and_validate_file_system(hDevice, sb)) {
        initialize_superblock(sb);
        memset(bitmap, 0, sizeof(bitmap));
        for(int i=0; i <= 9; i++) set_block_used(i);
        for(int i=0; i < 128; i++) file_table[i].in_use = false;
        save_superblock(hDevice,sb);
        save_bitmap(hDevice);
        save_file_table(hDevice);
        cout << "Filesystem initialized." << endl;
    } else {
        cout << "Filesystem loaded, version: " << sb.version << endl;
    }

    // Initialize Virtual Memory Manager with USB device as swap
    VirtualMemoryManager vmm(hDevice);

    // Initialize Scheduler with desired quantum
    Scheduler scheduler(2);

    // Example: Add sample processes to scheduler
    scheduler.add_fcfs({1,"ProcA",5,1,5});
    scheduler.add_rr({2,"ProcB",8,2,8});
    scheduler.add_priority({3,"ProcC",4,1,4});

    // Main OS loop (simplified)
    bool running = true;
    while(running) {
        cout << "OS Kernel Main Loop. Commands: fs, mem, sched, exit" << endl;
        string cmd; 
        getline(cin, cmd);

        if(cmd == "exit") {
            running = false;
            cout << "Exiting OS Kernel." << endl;
        } else if(cmd == "fs") {
            // Enter filesystem CLI
            interactive_cli(hDevice);
        } else if(cmd == "mem") {
            // Test virtual memory manager page accesses
            cout << "Access page number: ";
            int pagenum; cin >> pagenum; cin.ignore();
            if(vmm.access_page(pagenum))
                cout << "Page " << pagenum << " accessed successfully.\n";
            else
                cout << "Page access failed.\n";
        } else if(cmd == "sched") {
            // Run different scheduling algorithms as demo
            cout << "Running FCFS scheduling...\n";
            scheduler.run_fcfs();
            cout << "Running Round Robin scheduling...\n";
            scheduler.run_rr();
            cout << "Running Priority scheduling...\n";
            scheduler.run_priority();
        } else {
            cout << "Unknown command." << endl;
        }
    }

    CloseHandle(hDevice);
    return 0;
}

