#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FourthScriptExample");

int receive = 0;
const int  NODE_NUM = 10;

void ReceivePacket (Ptr<Socket> socket)
{
  NS_LOG_UNCOND("ReceivePacket");

//  while (socket->Recv())
//  {
    Ptr<Packet> packet;
    Address from;
    packet = socket->RecvFrom(from);
    receive++;

    NS_LOG_UNCOND (receive << " packet is arrived from " << InetSocketAddress::ConvertFrom (from).GetIpv4() << " with " << packet->GetSize() << " bytes");
//  }
}

static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval)
{
  NS_LOG_UNCOND("GenerateTraffic");

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
//  uint32_t queueSize = 10;

  CommandLine cmd;

  cmd.AddValue ("PacketSize", "Size of application packet", packetSize);
  cmd.AddValue ("PacketNumber", "Number of application packet", packetNumber);
  cmd.AddValue ("Interval", "Interval between packets", interval);

  cmd.Parse (argc, argv);
 
  Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpNewReno::GetTypeId()));

  Time interPacketInterval = Seconds (interval);

  NodeContainer nodes;
  nodes.Create (NODE_NUM);

/*
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);
*/

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("5Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds(2000)));

  NetDeviceContainer devices;
  devices = csma.Install(nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");	

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  TypeId tid = TypeId::LookupByName ("ns3::TcpSocketFactory");

  Ptr<Socket> recvSink;
//  InetSocketAddress local = InetSocketAddress(interfaces.GetAddress (0), 9);

  Ptr<Socket> source;
//  InetSocketAddress remote = InetSocketAddress(interfaces.GetAddress (0), 9);

  for(int i=0; i<NODE_NUM; i++) {

    recvSink = Socket::CreateSocket (nodes.Get(i), tid);
    InetSocketAddress local = InetSocketAddress (interfaces.GetAddress (i), 9);
    recvSink->Bind (local);
    recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

    if(i!=0) {
      source = Socket::CreateSocket (nodes.Get(i), tid);
      InetSocketAddress remote = InetSocketAddress (interfaces.GetAddress (i-1), 9);
      source->Connect (remote);
	
      Simulator::ScheduleWithContext (source->GetNode ()->GetId (), Seconds (1.0), &GenerateTraffic, source, packetSize, packetNumber, interPacketInterval);
    }
  }

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
