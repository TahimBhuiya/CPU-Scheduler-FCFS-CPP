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

    while (!ready_queue.empty() || !io_list.empty()) {
        // Move I/O-completed processes back to ready queue
        vector<pair<Process*, int>> still_in_io;
        for (auto& [p, t] : io_list) {
            if (t <= current_time) {
                ready_queue.push(p);
                p->last_end_time = t;
            } else {
                still_in_io.push_back({p, t});
            }
        }
        io_list = still_in_io;

        Process* running_process = ready_queue.empty() ? nullptr : ready_queue.front();
        display_status(current_time, running_process, ready_queue, io_list);
        if (!ready_queue.empty()) {
            Process* process = ready_queue.front(); ready_queue.pop();

            if (process->response_time == -1)
                process->response_time = current_time;

            int waiting_since_last = current_time - process->last_end_time;
            if (waiting_since_last > 0)
                process->waiting_time += waiting_since_last;

            int burst_time = process->burst_times[process->current_burst];
            current_time += burst_time;
            cpu_busy_time += burst_time;
            process->current_burst++;

            if (process->current_burst < process->burst_times.size()) {
                int io_time = process->io_times[process->current_burst - 1];
                io_list.push_back({process, current_time + io_time});
            } else {
                process->turnaround_time = current_time;
                completed_processes.push_back(process);
                cout << "Process P" << process->pid << " has completed its total execution." << endl;
            }

        } else {
            if (!io_list.empty()) {
                int next_io_completion = min_element(io_list.begin(), io_list.end(),
                    [](auto& a, auto& b) { return a.second < b.second; })->second;
                current_time = next_io_completion;
            } else break; // Nothing left to process
        }
    }

    int total_time = current_time;
    double cpu_utilization = total_time > 0 ? (cpu_busy_time * 100.0 / total_time) : 0.0;

    return {completed_processes, cpu_utilization, total_time};
}
