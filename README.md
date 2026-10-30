# TCP Network Congestion Control Simulator

## Project Overview
A Computer Networks project that simulates TCP congestion control mechanisms and demonstrates how TCP responds to packet transmission and packet loss.

## Features
- Packet transmission simulation
- Random packet loss
- TCP Slow Start
- TCP Congestion Avoidance
- Simplified TCP Reno-like recovery
- Congestion window (cwnd) tracking
- Packet statistics and retransmission counting
- CSV output for congestion window data

## Technologies Used
- C++
- Python
- Git and GitHub
- GNUplot (for graph generation, subject to integration)

## Project Structure
- `main.cpp` – C++ simulation engine
- `app.py` – Python application
- `cwnd_data.csv` – Simulation output data
- Frontend files – Browser interface, if included

## How to Compile and Run

Compile the C++ program:

```bash
g++ main.cpp -o simulator.exe
```

Run the executable:

```bash
.\simulator.exe
```

Run the Python application:

```bash
python app.py
```

## Note
This is an educational simulation. Its Reno-like recovery mechanism approximates duplicate ACK and timeout behaviour rather than implementing a complete TCP protocol stack.
