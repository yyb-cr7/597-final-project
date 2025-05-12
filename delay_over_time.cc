#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/aodv-module.h"
#include "ns3/dsdv-module.h"
#include "ns3/zrp-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("DelayOverTimeSimulation");

int main(int argc, char *argv[])
{
    double simulationTime = 300.0;
    uint32_t numNodes = 50;
    double nodeSpeed = 10.0;
    double interval = 10.0;

    NodeContainer nodes;
    nodes.Create(numNodes);

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211b);

    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    wifiPhy.SetChannel(wifiChannel.Create());

    WifiMacHelper wifiMac;
    wifiMac.SetType("ns3::AdhocWifiMac");

    NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, nodes);

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::RandomWaypointMobilityModel",
                              "Speed", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + std::to_string(nodeSpeed) + "]"),
                              "Pause", StringValue("0.5s"));

    mobility.SetPositionAllocator("ns3::RandomRectanglePositionAllocator",
                                  "X", StringValue("ns3::UniformRandomVariable[Min=0|Max=1000]"),
                                  "Y", StringValue("ns3::UniformRandomVariable[Min=0|Max=1000]"));

    mobility.Install(nodes);

    InternetStackHelper internet;
    AodvHelper aodv;
    internet.SetRoutingHelper(aodv);
    internet.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    uint16_t port = 9;
    OnOffHelper onoff("ns3::UdpSocketFactory", InetSocketAddress(interfaces.GetAddress(numNodes - 1), port));
    onoff.SetConstantRate(DataRate("16kbps"), 512);

    ApplicationContainer apps = onoff.Install(nodes.Get(0));
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(simulationTime));

    PacketSinkHelper sink("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
    apps = sink.Install(nodes.Get(numNodes - 1));
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(simulationTime));

    FlowMonitorHelper flowmonHelper;
    Ptr<FlowMonitor> flowMonitor = flowmonHelper.InstallAll();

    Simulator::Stop(Seconds(simulationTime));

    for(double time = interval; time <= simulationTime; time += interval)
    {
        Simulator::Schedule(Seconds(time), [&flowMonitor, time]() {
            flowMonitor->CheckForLostPackets();
            Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowMonitor->GetClassifier());
            std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();

            for(auto iter = stats.begin(); iter != stats.end(); ++iter)
            {
                Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);
                double delay = iter->second.delaySum.GetSeconds() / iter->second.rxPackets;
                NS_LOG_UNCOND("Time: " << time << "s, Average Delay: " << delay << " s");
            }
        });
    }

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
