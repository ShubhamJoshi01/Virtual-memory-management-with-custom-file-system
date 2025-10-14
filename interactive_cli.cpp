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

            
        //     string filename;
        //     int size;
        //     istringstream iss(cmd.substr(7));
        //     iss>>filename>>size;
        //     if(size>0 && create_file(hDevice,filename.c_str(),size)) cout<<"File created"<<endl;
        //     else cout<<"Failed to create file"<<endl;
        // }
        else if(cmd.substr(0,4)=="read"){
            string filename=cmd.substr(5);
            char buffer[BLOCK_SIZE*128]={0};
            if(read_file(hDevice,filename.c_str(),buffer,sizeof(buffer))){
                cout<<"File content read successfully (raw binary output not shown)"<<endl;
            } else cout<<"Failed to read file"<<endl;
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