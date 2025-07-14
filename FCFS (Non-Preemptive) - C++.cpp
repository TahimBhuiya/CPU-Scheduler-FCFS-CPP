//Created by Tahim Bhuiya


#include <iostream>   // for input/output operations
#include <vector>     // for dynamic arrays (std::vector)
#include <queue>      // for queue data structure (std::queue)
#include <tuple>      // for returning multiple values (std::tuple)
#include <iomanip>    // for formatted output (std::setprecision, std::setw)
#include <algorithm>  // for algorithms like std::min_element



using namespace std;  // Allow usage of standard library names without std:: prefix

// Class representing a process with CPU bursts and I/O times
class Process {
public:
    int pid;                    // Process ID
    vector<int> burst_times;    // List of CPU burst durations
    vector<int> io_times;       // List of I/O durations (one less than bursts)
    int current_burst = 0;      // Index of the current CPU burst
    int waiting_time = 0;       // Total waiting time accumulated
    int turnaround_time = 0;    // Total turnaround time for the process
    int response_time = -1;     // Time when process first got CPU (-1 if not started)
    int last_end_time = 0;      // Last time process finished CPU or I/O

    // Constructor to initialize process ID, CPU bursts, and I/O times
    Process(int id, const vector<int>& bursts, const vector<int>& ios)
        : pid(id), burst_times(bursts), io_times(ios) {}

    // Check if all CPU bursts have been completed
    bool is_completed() const {
        return current_burst >= burst_times.size();
    }
};



// Display the current simulation status including:
// - The current simulation time
// - The process currently running on the CPU (if any)
// - The contents of the ready queue
// - The list of processes currently performing I/O and their remaining I/O times
void display_status(
    int current_time,
    Process* running_process,
    queue<Process*> ready_queue,
    vector<pair<Process*, int>> io_list
) {
    // Print the current simulation time
    cout << "\nCurrent Execution Time: " << current_time << endl;

    // Print the currently running process or indicate if CPU is idle
    if (running_process)
        cout << "Running Process: P" << running_process->pid << endl;
    else
        cout << "Running Process: None" << endl;


    // Display the processes currently in the ready queue along with their next CPU burst time
    cout << "Ready Queue: [";
    while (!ready_queue.empty()) {
        Process* p = ready_queue.front(); 
        ready_queue.pop();
        cout << "(P" << p->pid << ", " << p->burst_times[p->current_burst] << ") ";
    }
    cout << "]" << endl;

    // Display the processes currently performing I/O with their remaining I/O time
    cout << "Processes in I/O: [";
    for (auto& [p, t] : io_list) {
        cout << "(P" << p->pid << ", " << t - current_time << ") ";
    }
    cout << "]" << endl;
}


// FCFS scheduling simulation with I/O handling.
// Returns a tuple containing:
// - vector of completed processes with updated metrics,
// - CPU utilization percentage,
// - total time taken for all processes to complete.
tuple<vector<Process*>, double, int> fcfs_scheduling(vector<Process*>& processes) {
    int current_time = 0;               // Simulation clock starting at time 0
    queue<Process*> ready_queue;        // Queue holding processes ready to execute
    for (auto* p : processes) 
        ready_queue.push(p);            // Initialize ready queue with all processes

    vector<pair<Process*, int>> io_list;       // List of processes currently in I/O with their completion times
    vector<Process*> completed_processes;      // List to store processes that have completed execution
    int cpu_busy_time = 0;                      // Total time CPU is actively executing processes


    // Continue simulation while there are processes either ready or performing I/O
    while (!ready_queue.empty() || !io_list.empty()) {
        // Check all processes in I/O and move those whose I/O is complete back to the ready queue
        vector<pair<Process*, int>> still_in_io;  // Temporary list for processes still in I/O
        for (auto& [p, t] : io_list) {
            if (t <= current_time) {
                // I/O complete, move process back to ready queue
                ready_queue.push(p);
                p->last_end_time = t;  // Update last end time to I/O completion time
            } else {
                // Process still performing I/O, keep it in the list
                still_in_io.push_back({p, t});
            }
        }

        // Update the I/O list with only those processes still performing I/O
        io_list = still_in_io;

        // Identify the process currently running on the CPU (front of ready queue)
        Process* running_process = ready_queue.empty() ? nullptr : ready_queue.front();
        // Display the current simulation status
        display_status(current_time, running_process, ready_queue, io_list);

        if (!ready_queue.empty()) {
            // Dequeue the next process to run (FCFS)
            Process* process = ready_queue.front(); 
            ready_queue.pop();

            // Record response time if this is the first CPU access for the process
            if (process->response_time == -1)
                process->response_time = current_time;

            // Calculate waiting time since last CPU or I/O completion
            int waiting_since_last = current_time - process->last_end_time;
            if (waiting_since_last > 0)
                process->waiting_time += waiting_since_last;

            // Fetch current CPU burst time and advance simulation clock
            int burst_time = process->burst_times[process->current_burst];
            current_time += burst_time;
            cpu_busy_time += burst_time;
            process->current_burst++;  // Move to next burst

            if (process->current_burst < process->burst_times.size()) {
                // Schedule next I/O operation after this CPU burst
                int io_time = process->io_times[process->current_burst - 1];
                io_list.push_back({process, current_time + io_time});
            } else {
                // Process has completed all CPU bursts
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


    cout << left << setw(12) << "\nProcess" << setw(10) << "Tw" << setw(12) << "Ttr" << setw(10) << "Tr" << endl;
    for (auto* p : completed_processes) {
        cout << "P" << left << setw(11) << p->pid
             << setw(10) << p->waiting_time
             << setw(12) << p->turnaround_time
             << setw(10) << p->response_time << endl;
    }

    cout << left << setw(12) << "\nAverage"
        << setw(10) << avg_waiting_time
        << setw(12) << avg_turnaround_time
        << setw(10) << avg_response_time << endl;

            
    for (auto* p : processes) delete p;

    return 0;
}

