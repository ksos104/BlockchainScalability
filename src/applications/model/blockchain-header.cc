#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/header.h"
#include "ns3/simulator.h"
#include "blockchain-header.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("BlockchainHeader");

NS_OBJECT_ENSURE_REGISTERED (BlockchainHeader);

BlockchainHeader::BlockchainHeader ()
  : m_a (0),
    m_b (0),
    m_c (0),
    m_d (0)
{
  NS_LOG_FUNCTION (this);
}

BlockchainHeader::~BlockchainHeader ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
BlockchainHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BlockchainHeader")
    .SetParent<Header> ()
    .SetGroupName("Applications")
    .AddConstructor<BlockchainHeader> ()
  ;
  return tid;
}

TypeId
BlockchainHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
BlockchainHeader::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 1+2+4+8;
}

void
BlockchainHeader::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this << &start);
  Buffer::Iterator i = start;
  i.WriteU8 (m_a);
  i.WriteHtonU16 (m_b);
  i.WriteHtonU32 (m_c);
  i.WriteHtonU64 (m_d);
}

uint32_t
BlockchainHeader::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this << &start);
  Buffer::Iterator i = start;
  m_a = i.ReadU8 ();
  m_b = i.ReadNtohU16 ();
  m_c = i.ReadNtohU32 ();
  m_d = i.ReadNtohU64 ();

  return GetSerializedSize ();
}

void
BlockchainHeader::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
}

} // namespace ns3

