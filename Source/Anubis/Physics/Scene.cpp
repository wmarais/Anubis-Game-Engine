#include "../../../Include/Anubis/Physics/Scene.hpp"

using namespace Anubis::Common;
using namespace Anubis::Physics;

/******************************************************************************/
void Scene::syncChildren(const Node * srcParent, Node * dstParent)
{
  /* Reserve space for all the children. */
  dstParent->fChildren.reserve(srcParent->fChildren.size());

  /* Create each of the children. */
  for(size_t i = 0; i < srcParent->fChildren.size(); i++)
  {
    /* Create the new child node and set it's parent and data.*/
    dstParent->fChildren.push_back(
          std::make_unique<Node>(dstParent,
                                 srcParent->fChildren[i]->fData));

    /* Now sync the children of the current child. */
    syncChildren(srcParent->fChildren[i].get(),
                 dstParent->fChildren[i].get());
  }
}

/******************************************************************************/
void Scene::sync(const Scene * scene)
{
  /* If there is no scene, then there is nothing to sync. */
  if(scene == nullptr)
    return;

  /* Check if the root nodes has the same data. */
  if(fRootNode->fData == scene->fRootNode->fData)
  {
  }

  /* Create a new root node for the scene. */
  fRootNode = std::make_unique<Node>(nullptr, scene->fRootNode->fData);

  /* Sync up the children of the scene. */
  syncChildren(scene->fRootNode.get(), fRootNode.get());
}

/******************************************************************************/
bool Scene::insert(const UUID & parentID,
                   std::shared_ptr<Common::SubObj> data)
{
  /* Find where the node must be inserted. */
  Node * insertPos = nullptr; //find(parentID);

  /* Check whether the result is valid. */
  if(insertPos == nullptr && parentID != kNullUUID)
  {
    /* This is an error since a valid parent is expected. */
    return false;
  }

  /* Create the new node to insert. */
  std::unique_ptr<Node> node = std::make_unique<Node>(insertPos, data);

  /* Check if it's the root node. */
  if(insertPos == nullptr)
  {
    /* Ceck if there is allready a root node. */
    if(fRootNode)
    {
      /* Add the current root node as a child to this node. */
      node->addChild(fRootNode);
    }

    /* Set the node as the new root node. */
    fRootNode = std::move(node);
  }
  /* Otherwise insert the node at the position. */
  {
    insertPos->fChildren.push_back(std::move(node));
  }

  /* Check if the change should be tracked. */
  if(fTrackChanges)
  {
    /* Record the action. */
    fChangeHistory.push_back(ChangeRecord(ChangeRecord::Types::kInsert,
                                          parentID, data));
  }

  /* Return true to indicate that the insert was sucessful. */
  return true;
}

/******************************************************************************/
bool Scene::remove(const UUID & nodeID)
{
  /* Find where the node that must be removed. */
  Node * removePos = nullptr; //find(nodeID);

  /* Check if the node was found. */
  if(removePos == nullptr)
  {
    /* This is an error since the node does not exist in the tree. */
    return false;
  }

  /* Check if the change should be tracked. */
  if(fTrackChanges)
  {
    /* Record the action. */
    fChangeHistory.push_back(ChangeRecord(ChangeRecord::Types::kRemove,
                                          nodeID, nullptr));
  }

  /* Return true to indicate the node was removed. */
  return true;
}

