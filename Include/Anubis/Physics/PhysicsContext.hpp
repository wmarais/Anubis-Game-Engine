#ifndef ANUBIS_PHYSICS_PHYSICS_CONTEXT_HPP
#define ANUBIS_PHYSICS_PHYSICS_CONTEXT_HPP

#include "../Common.hpp"
#include "Scene.hpp"

namespace Anubis
{
  namespace Physics
  {
    /***********************************************************************//**
     * A context object that store all the relevant information for the physics
     * threads. The physics context are relatively unique in that they store
     * a back and front buffer scene. The main physics processing is performed
     * on the back buffer scene and is synced with the front buffer scene
     * when available.
     **************************************************************************/
    class PhysicsContext : public Common::Context
    {
      /** Mutex to control access to the back scene. */
      std::mutex fFrontSceneMutex;

      /** The scene object that physics simulation acts upon. */
      std::unique_ptr<Scene> fBackScene;

      /** The scene object that the AI and Render Queue is calcualte from. */
      std::unique_ptr<Scene> fFrontScene;

      /*********************************************************************//**
       * Synchronise the front and back scene objects. This function is only
       * called after a full simulation update and as such the back scene is
       * safe for access in this function.
       *
       * @return  True if the two scenes were synchronised, else false.
       ************************************************************************/
      bool sync();

      virtual void update(float dt);

    public:

      /*********************************************************************//**
       * Create a new physics context object and set the tick rate / update
       * interval of the simulation.
       *
       * @param tickRate
       */
      PhysicsContext(const std::chrono::nanoseconds & updateRate =
          std::chrono::nanoseconds(8333333));

      virtual ~PhysicsContext();

    };
  }
}

#endif /* ANUBIS_PHYSICS_PHYSICS_CONTEXT_HPP */
