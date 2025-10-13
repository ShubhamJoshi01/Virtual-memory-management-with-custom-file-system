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
    if(!load_and_validate_file_system(hDevice,sb)){
        cout<<"Failed to load or validate file system."<<endl;
        return;
    }
    string cmd;
    while(true){
        cout<<"fs > ";
        getline(cin,cmd);
        if(cmd=="exit") break;
        else if(cmd=="list") list_files();
        else if(cmd.substr(0,6)=="create"){
            string filename;
            int size;
            istringstream iss(cmd.substr(7));
            iss>>filename>>size;
            if(size>0 && create_file(hDevice,filename.c_str(),size)) cout<<"File created"<<endl;
            else cout<<"Failed to create file"<<endl;
        }
        else if(cmd.substr(0,4)=="read"){
            string filename=cmd.substr(5);
            char buffer[BLOCK_SIZE*128]={0};
            if(read_file(hDevice,filename.c_str(),buffer,sizeof(buffer))){
                cout<<"File content read successfully (raw binary output not shown)"<<endl;
            } else cout<<"Failed to read file"<<endl;
        }
        else cout<<"Unknown command"<<endl;
    }
}