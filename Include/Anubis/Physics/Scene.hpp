#ifndef ANUBIS_PHYSICS_SCENE_HPP
#define ANUBIS_PHYSICS_SCENE_HPP

#include "../Common/Misc.hpp"
#include "../Common/UUID.hpp"
#include "../Math/Matrix4f.hpp"
#include "BoundingVolume.hpp"
 #include "../Common/SubObj.hpp"

namespace Anubis
{
  namespace Physics
  {
    /***********************************************************************//**
     * The scene graph that describes the environment. Note that the scene is
     * not directly renderable.
     **************************************************************************/
    class Scene
    {
      /*********************************************************************//**
       * A node in the scene's tree structure.
       ************************************************************************/
      struct Node //: public Common::SubObj
      {
        /** The parent of this node in the tree. If this parent is null, then
         * this is the root node of the tree. */
        Node * fParent;

        /** The list of children of this node. */
        std::vector<std::unique_ptr<Node>> fChildren;

        /** The subobject contained in this node. */
        std::shared_ptr<Common::SubObj> fData;

        /** The transformation of this node relative to it's parent. */
        Math::Matrix4f fTransform;

        /** The transform of this node relative to the world. */
        Math::Matrix4f fWorldTransform;

        /*******************************************************************//**
         * Create a node with with no children and the specified parent.
         * @param parent
         **********************************************************************/
        ANUBIS_FORCE_INLINE Node(Node * parent = nullptr,
                                 std::shared_ptr<Common::SubObj> subObj = nullptr) :
          fParent(parent) {}

        ANUBIS_FORCE_INLINE Node(Node * parent = nullptr) :
          fParent(parent) {}

//        /*******************************************************************//**
//         * Create a scene node with the. Note that the node will not take
//         * ownership of the parent pointer, but will for all the child pointers.
//         * This prevent's cycling references but is still safe to use since the
//         * ownership of the parent node is controlled by the parent's parent.
//         * Thus unless there is a bug in the tree code, all operations are safe.
//         *
//         * @param parent    The parent node of this node.
//         * @param children  The child nodes of this node. Note that the vector
//         *                  of children that is passed in will no longer be
//         *                  valid after this function has been executed.
//         **********************************************************************/
//        ANUBIS_FORCE_INLINE Node(Node * parent,
//          std::vector<std::unique_ptr<Node>> & children) :
//              fParent(parent)
//        {
//          /* Reserve space for all the children. */
//          fChildren.reserve(children.size());

//          /* Iterate through all the children and move the ownership to this
//           * node. */
//          for(size_t i = 0; i < children.size(); i++)
//          {
//            fChildren.push_back(std::move(children[i]));
//          }

//          /* Clear the orignal vector since none of the pointers in it will be
//           * valid anymore. */
//          children.clear();
//        }

        /*******************************************************************//**
         * Empty virtual destructor to ensure that any subclass' destrutor will
         * be invoked.
         **********************************************************************/
        ANUBIS_FORCE_INLINE virtual ~Node() {}

        /*******************************************************************//**
         * Return true if the node is the root node of the tree (i.e. has no
         * parent node) or false if it's not (i.e. has a parent node).
         *
         * @return  True if it's the root node or false if not.
         **********************************************************************/
        ANUBIS_FORCE_INLINE bool isRoot() const
        {
          return fParent == nullptr;
        }

        ANUBIS_FORCE_INLINE void addChild(std::unique_ptr<Node> & child)
        {
          /* Set the parent of the child. */
          child->fParent = this;

          /* Add the child to this node's children. */
          fChildren.push_back(std::move(child));
        }

        /*******************************************************************//**
         * Return true if the current node is a leaf node (no children) or
         * return false if it is not (has children).
         *
         * @return  True if it's a leaf node or False if it's not.
         **********************************************************************/
        ANUBIS_FORCE_INLINE bool isLeaf() const
        {
          return fChildren.size() == 0;
        }

        /*******************************************************************//**
         * Indicate if the the node is renderable. This is only ever true if
         * the object has a mesh and is in the camera's frustum.
         *
         * @return
         **********************************************************************/
        ANUBIS_INLINE virtual bool isVisible() const
        {
          return false;
        }
      };


      /*********************************************************************//**
       * In order to optimise tree syncrhonisation, instead of rebuilding the
       * tree each frame, a list of changes are tracked and a sync action
       * simply has to implement these changes.
       ************************************************************************/
      struct ChangeRecord
      {
        /** The diffirent types of scene changes. */
        enum class Types
        {
          kInsert,
          kRemove,
        };

        /** The change the occured. */
        Types fType;

        /* If it was a remove, this is the node that was removed, if it was an
         * insert, then this is the parent node that was inserted. */
        Common::UUID fID;

        /* If it was an insert, then this is the data that was inserted, else
         * nullptr. */
        std::shared_ptr<Common::SubObj> fData;

        ChangeRecord(Types type, const Common::UUID & id,
                     std::shared_ptr<Common::SubObj> data) : fType(type),
          fID(id), fData(data) {}

      };

      /** The history of changes that occured since the last time a sync was
       * performed. */
      std::vector<ChangeRecord> fChangeHistory;

    protected:

      /** Indicate if a change histry should be kept. Without it, changes
       * will not be tracked. */
      bool fTrackChanges;

      /** The root node of the tree. */
      std::unique_ptr<Node> fRootNode;


      size_t getNodeCount() const { return 0; }


      /*********************************************************************//**
       * Sync the child nodes of the srcParent node to the dstParent node.
       *
       * @param srcParent The node where the children will be synced / mirrored
       *                  from.
       * @param dstParent The node where the children will be synced / mirrored
       *                  too.
       ***********************************************************************/
      void syncChildren(const Node * srcParent, Node *dstParent);


      Node * find(const Common::UUID & uuid);

    public:

      /*********************************************************************//**
       * Change to contents of this scene to reflect the contents of the
       * supplied scene.
       *
       * @param scene
       ************************************************************************/
      void sync(const Scene * scene);

      /*********************************************************************//**
       * Insert a new node in the scene with the specified data and with a
       * parent of the specified UUID. If the id == kNullUUID, then it implies
       * the item should be made the root node of the tree.
       *
       * @param id    The id of the parent node where the new node should be
       *              inserted.
       * @param data  The data to associate with the node.
       * @return      True if the node was inserted, false if thje parentID
       *              could not be located and the node was not inserted.
       ************************************************************************/
      bool insert(const Common::UUID & parentID,
                  std::shared_ptr<Common::SubObj> data);

      /*********************************************************************//**
       * Remove the node identified by the id from the scene graph.
       *
       * @param id  The ID of the node to remove.
       * @return    True if the node was found and removed, else false.
       ************************************************************************/
      bool remove(const Common::UUID & nodeID);

    };
  }
}


#endif /* ANUBIS_PHYSICS_SCENE_HPP */
