#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <iomanip>
#include <algorithm>


using namespace std;

class Process {
public:
    int pid;
    vector<int> burst_times;
    vector<int> io_times;
    int current_burst = 0;
    int waiting_time = 0;
    int turnaround_time = 0;
    int response_time = -1;
    int last_end_time = 0;

    Process(int id, const vector<int>& bursts, const vector<int>& ios)
        : pid(id), burst_times(bursts), io_times(ios) {}

    bool is_completed() const {
        return current_burst >= burst_times.size();
    }
};


void display_status(
    int current_time,
    Process* running_process,
    queue<Process*> ready_queue,
    vector<pair<Process*, int>> io_list
){
    cout << "\nCurrent Execution Time: " << current_time << endl;

    if (running_process)
        cout << "Running Process: P" << running_process->pid << endl;
    else
        cout << "Running Process: None" << endl;

    cout << "Ready Queue: [";
    while (!ready_queue.empty()) {
        Process* p = ready_queue.front(); ready_queue.pop();
        cout << "(P" << p->pid << ", " << p->burst_times[p->current_burst] << ") ";
    }
    cout << "]" << endl;

    cout << "Processes in I/O: [";
    for (auto& [p, t] : io_list) {
        cout << "(P" << p->pid << ", " << t - current_time << ") ";
    }
    cout << "]" << endl;
}

tuple<vector<Process*>, double, int> fcfs_scheduling(vector<Process*>& processes) {
    int current_time = 0;
    queue<Process*> ready_queue;
    for (auto* p : processes) ready_queue.push(p);

    vector<pair<Process*, int>> io_list;
    vector<Process*> completed_processes;
    int cpu_busy_time = 0;