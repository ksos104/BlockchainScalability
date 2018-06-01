#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FourthScriptExample");

int count = 1;

void ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;
  packet = socket->RecvFrom (from);
  NS_LOG_UNCOND("Receive " << count++ << "th Packet!");
}

static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval)
{
  if (pktCount > 0)
  {
    Ptr<Packet> p = Create<Packet> (pktSize);
    socket->Send (p);
    Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktSize, pktCount-1, pktInterval);
  }
  else
  {
    socket->Close();
  }
}

static void Progress ()
{
  NS_LOG_UNCOND ("Current Simulation Time = " << Simulator::Now().GetSeconds() << " s");
  Simulator::Schedule (Seconds(3), &Progress);
}

int main (int argc, char *argv[])
{
  uint32_t packetSize = 1024; //byte
  uint32_t packetNumber = 1000;
  double interval = 2; //second
  uint16_t totalTime = 310;

  CommandLine cmd;

  cmd.AddValue ("PacketSize", "Size of application packet", packetSize);
  cmd.AddValue ("PacketNumber", "Number of application packet", packetNumber);
  cmd.AddValue ("Interval", "Interval between packets", interval);

  cmd.Parse (argc, argv);
 
  Time interPacketInterval = Seconds (interval);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");	

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink = Socket::CreateSocket (nodes.Get(1), tid);
  InetSocketAddress local = InetSocketAddress (interfaces.GetAddress (1), 9);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

  Ptr<Socket> source = Socket::CreateSocket (nodes.Get(0), tid);
  InetSocketAddress remote = InetSocketAddress (interfaces.GetAddress (1), 9);
  source->Connect (remote);
	
  Simulator::ScheduleWithContext (source->GetNode ()->GetId (), Seconds (1.0), &GenerateTraffic, source, packetSize,
                                  packetNumber, interPacketInterval);

  Progress();
  Simulator::Stop (Seconds(totalTime));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
