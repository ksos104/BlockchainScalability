/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ScratchSimulator");

int 
main (int argc, char *argv[])
{
  //LogComponentEnable("ScratchSimulator", LOG_LEVEL_ERROR);
  //LogComponentEnable("ScratchSimulator", LOG_LEVEL_WARN);
  //LogComponentEnable("ScratchSimulator", LOG_LEVEL_DEBUG);
  //LogComponentEnable("ScratchSimulator", LOG_LEVEL_INFO);
  //LogComponentEnable("ScratchSimulator", LOG_LEVEL_FUNCTION);
  //LogComponentEnable("ScratchSimulator", LOG_LEVEL_LOGIC);
  //LogComponentEnable("ScratchSimulator", LOG_LEVEL_ALL);
  //LogComponentEnable("ScratchSimulator", LOG_ALL);

  NS_LOG_ERROR("Error");
  NS_LOG_WARN("Warn");
  NS_LOG_DEBUG("Debug");
  NS_LOG_INFO("Info");
  NS_LOG_FUNCTION("Function");
  NS_LOG_LOGIC("Logic");
  NS_LOG_UNCOND ("Scratch Simulator");

  Simulator::Run ();
  Simulator::Destroy ();
}
