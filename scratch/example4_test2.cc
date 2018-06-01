#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FourthScriptExample");

int receive = 0;

void ReceivePacket (Ptr<Socket> socket)
{
//  while (socket->Recv())
//  {
//    NS_LOG_UNCOND ("Received one packet!");

  Ptr<Packet> packet;
  Address from;
  packet = socket->RecvFrom(from);
  receive++;

  NS_LOG_UNCOND(receive << " packet is arrived from " << InetSocketAddress::ConvertFrom (from).GetIpv4() << " with " << packet->GetSize() << " bytes");

//  }
}

static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval)
{
  if (pktCount > 0)
  {
    socket->Send (Create<Packet> (pktSize));

    Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktSize, pktCount-1, pktInterval);
  }
  else
  {
    socket->Close();
  }
}

int main (int argc, char *argv[])
{
  uint32_t packetSize = 1024; //byte
  uint32_t packetNumber = 100;
  double interval = 1.0; //second

  CommandLine cmd;

  cmd.AddValue ("PacketSize", "Size of application packet", packetSize);
  cmd.AddValue ("PacketNumber", "Number of application packet", packetNumber);
  cmd.AddValue ("Interval", "Interval between packets", interval);

  cmd.Parse (argc, argv);
 
  Time interPacketInterval = Seconds (interval);

  NodeContainer CsmaNodes;
  CsmaNodes.Create (10);

  NodeContainer PtpNodes;
  PtpNodes.Create(2);


  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (PtpNodes);

  InternetStackHelper stack;
  stack.Install (PtpNodes);

  Ipv4AddressHelper address;
  address.SetBase ("20.1.1.0", "255.255.255.0");	

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink = Socket::CreateSocket (PtpNodes.Get(1), tid);
  InetSocketAddress local = InetSocketAddress (interfaces.GetAddress (1), 9);
  local = InetSocketAddress (interfaces.GetAddress (1), 8);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

  Ptr<Socket> source = Socket::CreateSocket (PtpNodes.Get(0), tid);
  InetSocketAddress remote = InetSocketAddress (interfaces.GetAddress (1), 8);
  source->Connect (remote);
	
  Simulator::ScheduleWithContext (source->GetNode ()->GetId (), Seconds (1.5), &GenerateTraffic, source, packetSize,
                                  packetNumber, interPacketInterval);


  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("5Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds(2000)));

//  NetDeviceContainer devices;
  devices = csma.Install(CsmaNodes);

//  InternetStackHelper stack;
  stack.Install (CsmaNodes);

//  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");	

//  Ipv4InterfaceContainer interfaces;
  interfaces  = address.Assign (devices);

//  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
//  Ptr<Socket> recvSink;
  recvSink  = Socket::CreateSocket (CsmaNodes.Get(1), tid);
//  InetSocketAddress local;
  local = InetSocketAddress (interfaces.GetAddress (1), 9);
//  local = InetSocketAddress (interfaces.GetAddress (1), 8);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

//  Ptr<Socket> source;
  source = Socket::CreateSocket (CsmaNodes.Get(0), tid);
//  InetSocketAddress remote;
  remote = InetSocketAddress (interfaces.GetAddress (1), 9);
  source->Connect (remote);
	
  Simulator::ScheduleWithContext (source->GetNode ()->GetId (), Seconds (1.0), &GenerateTraffic, source, packetSize,
                                  packetNumber, interPacketInterval);

  recvSink = Socket::CreateSocket (CsmaNodes.Get(2), tid);
  local = InetSocketAddress (interfaces.GetAddress (2), 9);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

  source = Socket::CreateSocket (CsmaNodes.Get(1), tid);
  remote = InetSocketAddress (interfaces.GetAddress (2), 9);
  source->Connect (remote);

  Simulator::ScheduleWithContext (source->GetNode ()->GetId (), Seconds (1.0), &GenerateTraffic, source, packetSize, packetNumber, interPacketInterval);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
