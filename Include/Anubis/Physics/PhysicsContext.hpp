#ifndef ANUBIS_PHYSICS_PHYSICS_CONTEXT_HPP
#define ANUBIS_PHYSICS_PHYSICS_CONTEXT_HPP

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
    class PhysicsContext
    {
      /** The tick rate / interval that physics must be updated. */
      const float kTickRate;

      /** Indicate if the thread is running or not. */
      std::atomic_bool fIsExecuting;

      /** The exception if one occured during executing of the thread. */
      std::exception_ptr fException;

      /** Mutex to control access to the back scene. */
      std::mutex fFrontSceneMutex;

      /** The scene object that physics simulation acts upon. */
      std::unique_ptr<Scene> fBackScene;

      /** The scene object that the AI and Render Queue is calcualte from. */
      std::unique_ptr<Scene> fFrontScene;

      void threadEntry();


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
      PhysicsContext(float tickRate = 1.0f/120.0f);

    };
  }
}

#endif /* ANUBIS_PHYSICS_PHYSICS_CONTEXT_HPP */
