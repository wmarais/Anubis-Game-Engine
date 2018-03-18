#include "../../../Include/Anubis/Graphics/Camera.hpp"
#include "../../../Include/Anubis/Graphics/MeshRef.hpp"
#include "../../../Include/Anubis/Graphics/Material.hpp"
#include "../../../Include/Anubis/Graphics/GLState.hpp"
#include "../../../Include/Anubis/Math/Quaternion.hpp"

#include "../../../Include/Anubis/Common/System.hpp"

using namespace Anubis::Math;
using namespace Anubis::Graphics;

/******************************************************************************/
void Camera::RenderQueue::reset()
{
  /* Clear the opaque queue. */
  fOpaqueQueue.clear();

  /* Clear the transparent queue. */
  fTransparentQueue.clear();
}

/******************************************************************************/
void Camera::RenderQueue::optimise()
{
  /* Iterate through all the opaque elements. */
  for(auto element : fOpaqueQueue)
  {
    /* Sort each the meshes such that those of the same buffer object are
     * grouped together. */
    std::sort(element.fMeshRefs.begin(), element.fMeshRefs.end(),
      [](const std::shared_ptr<MeshRef> & lhs,
         const std::shared_ptr<MeshRef> & rhs)
      {
        /* Since the mesh buffer object IDs are simply integers, a simple
         * comparison is adequate. */
        return lhs->operator <(rhs.get());
      });
  }
}

/******************************************************************************/
void Camera::RenderQueue::renderOpaque(const Camera * camera)
{
  /* Iterate through all the opaque elements. */
  for(auto element : fOpaqueQueue)
  {
    /* Activate the material. */
    element.fMaterial->activate(camera);

    /* Iterate throug the visible meshes and render them. */
    for(auto meshRef : element.fMeshRefs)
    {
      /* Render the mesh. */
      meshRef->render(camera);
    }
  }
}

/******************************************************************************/
void Camera::RenderQueue::renderTransparent(const Camera * camera)
{
  /* Iterate through all the transparent elements. */
  for(auto element : fTransparentQueue)
  {
    /* Activate the material. */
    element.fMaterial->activate(camera);

    /* Render only the visible elements. */
    element.fMeshRef->render(camera, element.fIndices);
  }
}

/******************************************************************************/
//void Camera::update(const Physics::Scene::Node * node,
//                    const Common::Library<LODSet> &lodLibrary)
//{
//  /* Check if the current node is visible. */
//  if(node->isVisible())
//  {
//    /* Find all the sub components of this component. */
// //   lodLibrary.findAllSubComps(node->getCompID());

//  }
//}

/******************************************************************************/
void Camera::update(const Physics::Scene & scene,
                    const Common::Library<LODSet> & lodLibrary)
{
  /* Clear the render queue. */
  fRenderQueue.reset();




//  /* TODO - Make this fast. */
//  /* Iterate through all the materials in the scene. */
//  for(auto material : scene->opaqueMaterials())
//  {
//    /* The list of visible meshes. */
//    std::vector<std::shared_ptr<MeshRef>> visibleMeshes;

//    /* Reserve enough memory to prevent allocation. */
//    visibleMeshes.reserve(material->meshRefs().size());

//    /* Iterate through all the meshes. */
//    for(auto meshRef : material->meshRefs())
//    {
//      /* Check if the mesh is visible to the camera. */
//      /* For now just make everything visible. It is more important to get
//       * something rendering that anything else atm. */
//      visibleMeshes.push_back(meshRef);

//      //fRenderQueue.
//    }
//  }
}

/******************************************************************************/
void Camera::render(GLState * state)
{
  /*  Enable depth testing. */
  state->setDepthTest(true);

  /* Render the Opaque meshes first. */
  fRenderQueue.renderOpaque(this);

  /* Disable Depth testing. */
  state->setDepthTest(false);

  /* Render the transparent meshes last. */
  fRenderQueue.renderTransparent(this);
}

/******************************************************************************/
Matrix4f Camera::calcTransform()
{
  /* Create a quaternion to produce a rotation matrix. */
  Quaternion quaternion;

}

/******************************************************************************/
void Camera::strafeUp(float step)
{
  /* Calculate the current camera transform. */
  Matrix4f transform = calcTransform();

  /* Move the position by the step amount. */
  fPosition += (transform.upVector() * step);
}

/******************************************************************************/
void Camera::strafeRight(float step)
{
  /* Calculate the current camera transform. */
  Matrix4f transform = calcTransform();

  /* Move the position by the step amount. */
  fPosition += (transform.rightVector() * step);
}

/******************************************************************************/
void Camera::strafeForward(float step)
{
  /* Calculate the current camera transform. */
  Matrix4f transform = calcTransform();

  /* Move the position by the step amount. */
  fPosition += (transform.headingVector() * step);
}

/******************************************************************************/
void Camera::rotateAboutX(float step)
{
  fXRotAngle = Float::sumAngles(fXRotAngle, step);
}

/******************************************************************************/
void Camera::rotateAboutY(float step)
{
  /* Add the step to the y rotation. */
  fYRotAngle = Float::sumAngles(fYRotAngle, step);
}

/******************************************************************************/
void Camera::rotateAboutZ(float step)
{
  /* Add the step to the z rotation. */
  fZRotAngle = Float::sumAngles(fZRotAngle, step);
}
