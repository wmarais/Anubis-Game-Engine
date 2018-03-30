#ifndef ANUBIS_PHYSICS_PHYSICS_CONTEXT_HPP
#define ANUBIS_PHYSICS_PHYSICS_CONTEXT_HPP

#include "../Common.hpp"
#include "Scene.hpp"

//namespace Anubis
//{
//  namespace Physics
//  {
//    /***********************************************************************//**
//     * A context object that store all the relevant information for the physics
//     * threads. The physics context are relatively unique in that they store
//     * a back and front scene object. The main physics processing is performed
//     * on the back scene object and the front scene object is used for
//     * rendering and AI functionality.
//     **************************************************************************/
//    class PhysicsContext : public Common::Context
//    {
//      /** Mutex to control access to the back scene. */
//      std::mutex fFrontSceneMutex;

//      /** The scene object that physics simulation acts upon. */
//      std::unique_ptr<Scene> fBackScene;

//      /** The scene object that the AI and Render Queue is calcualte from. */
//      std::unique_ptr<Scene> fFrontScene;

//      /*********************************************************************//**
//       * Synchronise the front and back scene objects. This function is only
//       * called after a full simulation update and as such the back scene is
//       * safe for access in this function.
//       *
//       * @return  True if the two scenes were synchronised, else false.
//       ************************************************************************/
//      bool sync();

//      virtual void update(float dt);


//      /*********************************************************************//**
//       *
//       ************************************************************************/
//      struct Operation
//      {
//        /** The diffirent types of operations. */
//        enum class Types
//        {
//          /** Operation to add a node to the scene. */
//          Insert,

//          /** Operation to remove a node from the scene. */
//          Remove
//        };

//        /** The type of action to be performed. */
//        Types fType;

//        /** The UUID of the parent object in the case of an insert, or the
//         * UUID of the node to be removed in the case of a removal. */
//        Common::UUID fObjID;

//        /** The data of the new node in the case of an insertion. */
//        std::shared_ptr<Common::SubObj> fData;

//        /** A simple constructor to quickly initialise the object. */
//        Operation(Types type, const Common::UUID & objID,
//          std::shared_ptr<Common::SubObj> data) : fType(type), fObjID(objID),
//          fData(data) {}
//      };

//      /** The mutex to lock access to the operations thread. */
//      std::mutex fOpQueueMutex;

//      /** The queue of operations to perform on the tree. */
//      std::vector<Operation> fOperations;

//      /** The maximum number of operatiosn to queue. */
//      const size_t kMaxOpQueueLen;

//    public:

//      /*********************************************************************//**
//       * Create a new physics context object and set the update rate of the
//       * simulation.
//       *
//       * @param updateRate      The update rate of the physics engine.
//       * @param maxOpeQueueLen  The maximum number of operations that can be
//       *                        queued.
//       ************************************************************************/
//      PhysicsContext(const std::chrono::nanoseconds & updateRate =
//          std::chrono::nanoseconds(8333333), size_t maxOpQueueLen = 4096);

//      virtual ~PhysicsContext();

//      /*********************************************************************//**
//       * Insert a node into the scene. This simply queues the action, it does
//       * not action the insertion until it is safe to do so.
//       *
//       * @param objID The ID of the parent object to add this node too. If it is
//       *              kNullUUID, then it is Root Element of the tree.
//       * @param data  The data of the object to be inserted.
//       * @return      True if the action was sucessfully queued, else False if
//       *              the queue was full and the action was not queued.
//       ************************************************************************/
//      bool insertNode(const Common::UUID & objID,
//                      std::shared_ptr<Common::SubObj> data);

//      /*********************************************************************//**
//       * Remove a node from the scene. This simply queues the action, it does
//       * not action the removal until it isafe to do so.
//       *
//       * @param objID The ID of the object to be removed. If it is kNullUUID,
//       *              then it is the Root Element of the tree.
//       * @return      True if the action was sucessfully queued, else False if
//       *              the queue was full and thea ction was not queued.
//       ************************************************************************/
//      bool removeNode(const Common::UUID & objID);

//    };
//  }
//}

#endif /* ANUBIS_PHYSICS_PHYSICS_CONTEXT_HPP */
