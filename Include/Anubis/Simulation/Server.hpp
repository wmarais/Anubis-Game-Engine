#ifndef ANUBIS_SIMULATION_SERVER_HPP
#define ANUBIS_SIMULATION_SERVER_HPP

#include "../Common.hpp"
#include "Context.hpp"

namespace Anubis
{
  namespace Simulation
  {
    class Server : public Context
    {
    protected:

      virtual void syncNetwork(bool isFirstFrame);
      virtual void updateNetwork(float dt, bool isFirstFrame);

      virtual void syncPhysics(bool isFirstFrame);
      virtual void updatePhysics(float dt, bool isFirstFrame);

      virtual void syncAI(bool isFirstFrame);
      virtual void updateAI(float dt, bool isFirstFrame);

    public:

      Server(const std::chrono::nanoseconds & updateRate =
          std::chrono::nanoseconds(8333333));

      virtual ~Server();
    };
  }
}

#endif/* ANUBIS_SIMULATION_SERVER_HPP */
