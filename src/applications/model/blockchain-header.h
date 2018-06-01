#ifndef BLOCKCHAIN_HEADER_H
#define BLOCKCHAIN_HEADER_H

#include "ns3/header.h"
#include "ns3/nstime.h"

namespace ns3 {

class BlockchainHeader : public Header
{
public:
  BlockchainHeader();
  ~BlockchainHeader();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

private:
  uint8_t m_a;
  uint16_t m_b;
  uint32_t m_c;
  uint64_t m_d;
};

}
#endif

