#include "../../../Include/Anubis/Simulation/Server.hpp"

using namespace std::chrono;
using namespace Anubis::Simulation;

/******************************************************************************/
Server::Server(const std::chrono::nanoseconds & updateRate) :
  Context(updateRate)

{
}

/******************************************************************************/
Server::~Server()
{
}

/******************************************************************************/
void Server::syncNetwork(bool isFirstFrame)
{
  ANUBIS_LOG_INFO("Sync   - Network");

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

/******************************************************************************/
void Server::updateNetwork(float dt, bool isFirstFrame)
{
  ANUBIS_LOG_INFO("Update - Network");
}

/******************************************************************************/
void Server::syncPhysics(bool isFirstFrame)
{
  ANUBIS_LOG_INFO("Sync   - Physics");
}

/******************************************************************************/
void Server::updatePhysics(float dt, bool isFirstFrame)
{
  ANUBIS_LOG_INFO("Update - Physics")
}

/******************************************************************************/
void Server::syncAI(bool isFirstFrame)
{
  ANUBIS_LOG_INFO("Sync   - AI");
}

/******************************************************************************/
void Server::updateAI(float dt, bool isFirstFrame)
{
  ANUBIS_LOG_INFO("Update - AI")
}


