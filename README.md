# 🧮 FCFS (Non-Preemptive) CPU Scheduling Simulation in C++  
**By Tahim Bhuiya**

This project implements a **First-Come, First-Served (FCFS) CPU Scheduling Algorithm** with support for **I/O bursts** in **C++**. It simulates process execution with alternating CPU and I/O bursts, and computes performance metrics like **waiting time**, **turnaround time**, **response time**, and **CPU utilization**.

---

## 📜 Overview

This simulation models how an operating system schedules processes using the **FCFS non-preemptive algorithm**, where:

- Each process has a list of CPU bursts and I/O times  
- Processes execute bursts in order  
- I/O-bound processes are temporarily removed from the ready queue  
- Time is advanced realistically to simulate CPU and I/O behavior  
- Metrics are tracked for all processes

---

## ▶️ Usage

Compile and run the C++ file:

```bash
g++ "FCFS (Non-Preemptive) - C++.cpp" -o fcfs_sim
./fcfs_sim
```

The output will include:

- Real-time status updates (running process, ready queue, and I/O list)  
- Completion logs for each process  
- A table with waiting, turnaround, and response times  
- CPU utilization and average metrics

---

## 🧠 Code Description

| Component                | Purpose                                                                 |
|--------------------------|-------------------------------------------------------------------------|
| `Process` class          | Holds burst times, I/O times, and performance metrics                   |
| `fcfs_scheduling()`      | Main simulation logic for FCFS with I/O handling                        |
| `display_status()`       | Prints current CPU status, ready queue, and I/O list                    |
| `is_completed()`         | Checks whether a process finished all its CPU bursts                   |
| Metric Computation       | Calculates average Tw, Ttr, Tr, and CPU usage percentage                |

---

## 🔧 Key Concepts

| Concept           | Description                                                                 |
|-------------------|-----------------------------------------------------------------------------|
| CPU Burst         | Time a process requires on the CPU                                          |
| I/O Time          | Time a process spends performing I/O between bursts                         |
| Ready Queue       | Queue of processes waiting for CPU (ordered by arrival)                     |
| I/O List          | Tracks processes currently performing I/O and their expected completion     |
| Response Time     | Time from arrival to first CPU execution                                    |
| Waiting Time      | Total time spent waiting in the ready queue                                 |
| Turnaround Time   | Total time from arrival to completion                                       |
| CPU Utilization   | % of total time the CPU was actively working                                |

---

## 📈 Example Output

```
Current Execution Time: 212
Running Process: P4
Ready Queue: [(6, 11), (5, 16), (7, 14), (8, 4)]
Processes in I/O: [(2, 25)]

Process P4 has completed its total execution.

Results at the end of the simulation:
Total time needed to complete all processes: 1012
CPU Utilization: 83.47%

Process     Tw        Ttr         Tr        
P1          63        198         0         
P2          112       312         12        
...

Average     84.75     256.50      15.38
```

---

## 📦 Requirements

- C++11 or higher  
- Standard libraries only (`<vector>`, `<queue>`, `<tuple>`, `<iomanip>`, `<iostream>`)

---

## ✅ Notes

- All processes are assumed to **arrive at time 0**.  
- I/O is **blocking** — a process returns to the ready queue only after I/O completes.  
- FCFS is strictly **non-preemptive** and **order-based**.  
- You can add or modify processes by editing their `burst_times` and `io_times`.

---

## 🛠️ How to Customize

To simulate different scenarios, simply add or modify the `Process` objects in `main()`:

```cpp
// Example:
processes.push_back(new Process(9, {5, 10, 6}, {12, 15}));
```

---

## 📣 Credits

Developed by **Tahim Bhuiya**  
Assignment – CPU Scheduling Simulation in C++

🖥️ Happy Scheduling!