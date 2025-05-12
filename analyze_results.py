import xml.etree.ElementTree as ET
import matplotlib.pyplot as plt

files = {
    "results/dsdv-results.xml": "DSDV",
    "results/aodv-results.xml": "AODV",
    "results/zrp-results.xml": "ZRP"
}

avg_delay = {}
avg_throughput = {}
packet_loss_rate = {}

for file, label in files.items():
    tree = ET.parse(file)
    root = tree.getroot()

    total_delay = 0
    total_rx_packets = 0
    total_tx_packets = 0
    total_rx_bytes = 0

    for flow in root.iter("FlowStats"):
        tx_packets = int(flow.attrib["txPackets"])
        rx_packets = int(flow.attrib["rxPackets"])
        delay_sum = float(flow.attrib["delaySum"].replace("ns", ""))  # 纳秒
        rx_bytes = int(flow.attrib["rxBytes"])
        time_last_rx = float(flow.attrib["timeLastRxPacket"].replace("s", ""))
        time_first_tx = float(flow.attrib["timeFirstTxPacket"].replace("s", ""))
        duration = max(time_last_rx - time_first_tx, 0.0001)

        total_delay += delay_sum
        total_tx_packets += tx_packets
        total_rx_packets += rx_packets
        total_rx_bytes += rx_bytes

    if total_rx_packets > 0:
        avg_delay[label] = (total_delay / total_rx_packets) / 1e6  # 转换为 ms
    else:
        avg_delay[label] = 0

    avg_throughput[label] = (total_rx_bytes * 8) / (duration * 1e6)  # Mbps
    packet_loss_rate[label] = 1 - (total_rx_packets / total_tx_packets) if total_tx_packets > 0 else 1.0


def plot_metric(metric_dict, title, ylabel):
    protocols = list(metric_dict.keys())
    values = list(metric_dict.values())

    plt.figure()
    plt.bar(protocols, values)
    plt.title(title)
    plt.ylabel(ylabel)
    plt.grid(axis='y')
    plt.show()

plot_metric(avg_delay, "Average End-to-End Delay", "Delay (ms)")
plot_metric(avg_throughput, "Average Throughput", "Throughput (Mbps)")
plot_metric(packet_loss_rate, "Packet Loss Rate", "Loss Rate")
