#include "../../../Include/Anubis/Physics/Scene.hpp"

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

