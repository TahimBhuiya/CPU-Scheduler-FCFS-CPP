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


int main() {
    vector<Process*> processes = {
        new Process(1, {5, 3, 5, 4, 6, 4, 3, 4}, {27, 31, 43, 18, 22, 26, 24}),
        new Process(2, {4, 5, 7, 12, 9, 4, 9, 7, 8}, {48, 44, 42, 37, 76, 41, 31, 43}),
        new Process(3, {8, 12, 18, 14, 4, 15, 14, 5, 6}, {33, 41, 65, 21, 61, 18, 26, 31}),
        new Process(4, {3, 4, 5, 3, 4, 5, 6, 5, 3}, {35, 41, 45, 51, 61, 54, 82, 77}),
        new Process(5, {16, 17, 5, 16, 7, 13, 11, 6, 3, 4}, {24, 21, 36, 26, 31, 28, 21, 13, 11}),
        new Process(6, {11, 4, 5, 6, 7, 9, 12, 15, 8}, {22, 8, 10, 12, 14, 18, 24, 30}),
        new Process(7, {14, 17, 11, 15, 4, 7, 16, 10}, {46, 41, 42, 21, 32, 19, 33}),
        new Process(8, {4, 5, 6, 14, 16, 6}, {14, 33, 51, 73, 87})
    };

    auto [completed_processes, cpu_utilization, total_time] = fcfs_scheduling(processes);

    int total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;
    int num_processes = completed_processes.size();

    for (auto* p : completed_processes) {
        total_waiting_time += p->waiting_time;
        total_turnaround_time += p->turnaround_time;
        total_response_time += p->response_time;
    }

    double avg_waiting_time = total_waiting_time / (double)num_processes;
    double avg_turnaround_time = total_turnaround_time / (double)num_processes;
    double avg_response_time = total_response_time / (double)num_processes;

    cout << "\nResults at the end of the simulation:\n";
    cout << "Total time needed to complete all processes: " << total_time << endl;
    cout << fixed << setprecision(2);
    cout << "CPU Utilization: " << cpu_utilization << "%" << endl;


