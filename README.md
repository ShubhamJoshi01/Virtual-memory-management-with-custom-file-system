# Virtual-memory-management-with-custom-file-system

This project simulates key operating system functionalities, including a custom file system, virtual memory management with paging and swap, and process scheduling, all implemented in user space on Windows. It leverages raw block-level access to a USB pendrive for storage, providing a practical, integrated environment to explore OS concepts without kernel modifications.

Features
Custom File System: Implements superblock, bitmap-based block allocation, file table, and basic file operations (create, read, delete, list).

Virtual Memory Management: Simulates larger virtual memory than physical RAM, with paging, page tables, swap space on pendrive, and page replacement algorithms (FIFO, LRU, Optimal).

Process Scheduling: Simulates common CPU scheduling algorithms (FCFS, Round Robin, Priority) in conjunction with file and memory operations.

Project Structure
superblock.h — Metadata storage structure.

bitmap.cpp — Block allocation bitmap management.

file_table.cpp — File entries handling.

virtual_memory.cpp — Paging and swap simulation.

scheduler.cpp — Process scheduling simulation.

main.cpp — integration of all modules.

Getting Started
Requires Windows with administrative privileges for raw USB access.

Developed in C++ using Windows API.

Compile using Visual Studio or compatible C++ compiler.

Run the executable as Administrator to access pendrive raw blocks.

Start by exploring file system initialization and reading/writing via CLI commands.

Usage
Interact via CLI commands like:

create, read, delete, ls — File system operations.

alloc, free, meminfo — Memory management.

run, ps — Process scheduling.

References
Silberschatz, Galvin, Gagne — Operating System Concepts

Tanenbaum — Modern Operating Systems

Microsoft Windows API Documentation
