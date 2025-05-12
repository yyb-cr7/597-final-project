# 597-final-project

This repository contains the final project for course **EE 597**, focusing on a **comparative simulation study** of three major MANET routing protocols: **DSDV**, **ZRP**, and **AODV**, implemented in a multihop wireless mesh network using **ns-3**.

---

## 📁 Project Structure

```
.
├── aodv-final.cc          # ns-3 simulation for AODV
├── dsdv-final.cc          # ns-3 simulation for DSDV
├── zrp-final.cc           # ns-3 simulation for ZRP
├── analyze_results.py     # Python script to parse and visualize results
├── makefile               # Compile and run automation
└── README.md              # Project documentation
```

---

## 📌 Objective

To analyze and compare the performance of three routing protocols under identical conditions.  
Key performance metrics include:

- 📦 Packet Delivery Ratio (PDR)  
- ⏱ End-to-End Delay  
- 🧾 Routing Overhead  
- 🚀 Throughput  

---

## ⚙️ Simulation Environment

- **Simulator**: ns-3 (version 3.39)  
- **Simulation Time**: 300 seconds  
- **Simulation Area**: 1000m × 1000m  
- **Number of Nodes**: 50  
- **Node Speed**: 0–20 m/s  
- **Mobility Model**: Random Waypoint  
- **Traffic Type**: CBR (UDP)  
- **Packet Size**: 512 bytes  
- **Packet Rate**: 4 packets/sec  
- **MAC/PHY**: IEEE 802.11b  

---

## How to Run

Compile all protocols:

```bash
make
```

Run each protocol simulation:

```bash
./aodv-final
./dsdv-final
./zrp-final
```

Analyze results and generate comparison plots:

```bash
python analyze_results.py
```

---

## 📊 Results Summary

| Metric               | Best Performer    |
| -------------------- | ----------------- |
| **PDR**              | AODV              |
| **End-to-End Delay** | AODV              |
| **Overhead**         | AODV < ZRP < DSDV |
| **Throughput**       | AODV              |

-  **AODV**: High delivery, low delay, low overhead — great for dynamic networks  
-  **ZRP**: Balanced overhead and delay due to hybrid routing  
-  **DSDV**: High overhead, less adaptive in mobile scenarios  

---

---

## ✍️ Author

Fucheng Zhu, Yanbo Yang

