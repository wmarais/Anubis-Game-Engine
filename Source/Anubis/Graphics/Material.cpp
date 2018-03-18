#include "../../../Include/Anubis/Graphics/Material.hpp"

using namespace Anubis::Graphics;

Material * Material::fActiveMaterial = nullptr;

Material::Material(std::shared_ptr<ShaderProgram> prog)
{

}


void Material::update(std::shared_ptr<Camera> camera)
{
  /* Sort the meshes based on:
   *   1. Visible First, Invisible Last.
   *   2. Order of ascending BufferObject ID number.
   *
   * Thus when the material renders the mesh, it only have to be concerned about
   * iterating over the visible meshes, and perform the least number of
   * buffer object (context) changes.
   *
   * TO TRY - Use std::partition first, then std::sort. This may improve
   * performance.*/
//  std::sort(fMeshes.begin(), fMeshes.end(),
//    [](const MeshRef & lhs, const MeshRef & rhs)
//    {
//      /* Check if the mesh is visible. */
//      if(lhs.isVisible() && rhs.isVisible())
//      {
//        /* Check which one is smaller. */
//        lhs.getMesh()
//      }
//    });
}

/******************************************************************************/
void Material::activate(const Camera * camera)
{
  /* Check if the material is allready active. Activating it again even if it
   * is allready active can cause a state change / graphics flush. */
  if(fActiveMaterial == this)
  {
    /* It is allready active, so don't activate it again. */
    return;
  }


}


