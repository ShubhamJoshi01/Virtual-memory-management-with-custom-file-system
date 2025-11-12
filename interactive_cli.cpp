#include <iostream>
#include <sstream>
#include <string>
#include "filesystem.h"
#include "file_ops.h"
#include "filesystem_recovery.h"
#include <windows.h>
using namespace std;

void interactive_cli(HANDLE hDevice){
    Superblock sb;
    string cmd;
    while(true){
        cout<<"fs > ";
        getline(cin,cmd);
        if(cmd=="exit") break;
        else if(cmd=="list") list_files();
        else if(cmd.substr(0,6)=="create"){
            if(cmd.size() <= 7) {
                cout << "Usage: create <filename> <size>" << endl;
                continue;
            }
            istringstream iss(cmd.substr(7));
            string filename;
            int size;
            iss >> filename >> size;
            if(size > 0 && create_file(hDevice, filename.c_str(), size))
                cout << "File created" << endl;
            else
                cout << "Failed to create file" << endl;
        }

        else if(cmd.substr(0,4)=="read"){
            string filename=cmd.substr(5);
            // char buffer[BLOCK_SIZE*128]={0};
            // if(read_file(hDevice,filename.c_str(),buffer,sizeof(buffer))){
            //     cout<<"File content read successfully (raw binary output not shown)"<<endl;
            // } else cout<<"Failed to read file"<<endl;
            int length_blocks = -1;
            for (int i = 0; i < 128; ++i) {
                if (file_table[i].in_use && strcmp(file_table[i].filename, filename.c_str()) == 0) {
                    length_blocks = file_table[i].length;
                    break;
                }
            }
            int buffer_size = length_blocks * BLOCK_SIZE;
            char* buffer = new char[buffer_size]();
            if (read_file(hDevice, filename.c_str(), buffer, buffer_size)) {
                cout << "File content read successfully:" << endl;
                buffer[buffer_size - 1] = '\0';  // null terminate safely
                cout << buffer << endl;
            }
            else {
                cout << "Failed to read file" << endl;
            }
            delete[] buffer;
        }
        else if (cmd.substr(0, 9) == "writefile") {
            // Format: writefile <filename> <data>
            istringstream iss(cmd.substr(10));
            string filename;
            iss >> filename;
            string data;
            getline(iss, data);
            size_t start = data.find_first_not_of(' ');
            if (start != string::npos) data = data.substr(start);
            else data = "";

            int size = (int)data.size();
            if (write_file(hDevice, filename.c_str(), data.c_str(), size)) {
                cout << "File written successfully" << endl;
            }
            else {
                cout << "Failed to write file" << endl;
            }
        }
        else if (cmd.substr(0,6) == "delete") {
            if (cmd.size() <= 7) {
                cout << "Usage: delete <filename>" << endl;
                continue;
            }
            string filename = cmd.substr(7); 
            if (delete_file(hDevice, filename.c_str()))
                cout << "File deleted successfully" << endl;
            else
                cout << "Failed to delete file (not found or error)" << endl;
        }
        

        else cout<<"Unknown command"<<endl;
    }
}