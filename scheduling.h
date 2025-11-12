// #include<iostream>
// #include <queue>
// #include <vector>
// #include <string>
// #include <algorithm>
// using namespace std;

// struct Process{
//     int pid;
//     string name;
//     int burst_time;
//     int priority;
//     int remaining_time;
// };

// class Scheduler{
//     queue<Process> fcfs_queue;
//     queue<Process> rr_queue;
//     vector<Process> priority_list;
//     int quantum;

// public:
//     Scheduler(int q=2):quantum(q){}

//     void add_fcfs(Process p){
//         fcfs_queue.push(p);
//     }

//     void add_rr(Process p){
//         p.remaining_time=p.burst_time;
//         rr_queue.push(p);
//     }

//     void add_priority(Process p){
//         priority_list.push_back(p);
//     }

//     void run_fcfs(){
//         while(!fcfs_queue.empty()){
//             Process p=fcfs_queue.front(); 
//             fcfs_queue.pop();
//             cout<<"Running process "<<p.pid<<" ("<<p.name<<") for "<<p.burst_time<<" units\n";
//         }
//     }

//     void run_rr(){
//         while(!rr_queue.empty()){
//             Process p=rr_queue.front();
//             int run_time=min(quantum,p.remaining_time);
//             cout<<"Running process "<<p.pid<<" ("<<p.name<<") for "<<run_time<<" units\n";
//             p.remaining_time-=run_time;
//             rr_queue.pop();
//             if(p.remaining_time>0) rr_queue.push(p);
//         }
//     }

//     void run_priority(){
//         sort(priority_list.begin(),priority_list.end(),[](Process a,Process b){return a.priority<b.priority;});
//         for(auto &p:priority_list)
//             cout<<"Running process "<<p.pid<<" ("<<p.name<<") with priority "<<p.priority<<" for "<<p.burst_time<<" units\n";
//     }
// };

// Scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

struct Process{
    int pid;
    std::string name;
    int burst_time;
    int priority;
    int remaining_time;
};

class Scheduler{
private:
    std::queue<Process> fcfs_queue;
    std::queue<Process> rr_queue;
    std::vector<Process> priority_list;
    int quantum;

public:
    Scheduler(int q=2):quantum(q){}
    void add_fcfs(Process p){ fcfs_queue.push(p); }
    void add_rr(Process p){ p.remaining_time = p.burst_time; rr_queue.push(p); }
    void add_priority(Process p){ priority_list.push_back(p); }

    void run_fcfs(){
        while(!fcfs_queue.empty()){
            Process p = fcfs_queue.front();
            fcfs_queue.pop();
            std::cout << "Running process " << p.pid << " (" << p.name << ") for " << p.burst_time << " units\n";
        }
    }

    void run_rr(){
        while(!rr_queue.empty()){
            Process p = rr_queue.front();
            int run_time = std::min(quantum, p.remaining_time);
            std::cout << "Running process " << p.pid << " (" << p.name << ") for " << run_time << " units\n";
            p.remaining_time -= run_time;
            rr_queue.pop();
            if(p.remaining_time > 0) rr_queue.push(p);
        }
    }

    void run_priority(){
        std::sort(priority_list.begin(), priority_list.end(), [](Process a, Process b){ return a.priority < b.priority;});
        for(auto &p : priority_list){
            std::cout << "Running process " << p.pid << " (" << p.name << ") with priority " << p.priority << " for " << p.burst_time << " units\n";
        }
    }
};

#endif
