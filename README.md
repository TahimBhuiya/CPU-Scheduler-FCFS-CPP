# 🧮 FCFS CPU Scheduling Simulation in C++  
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