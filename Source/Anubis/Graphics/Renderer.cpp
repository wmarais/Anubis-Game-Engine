#include "../../../Include/Anubis/Graphics/Renderer.hpp"

using namespace Anubis::Graphics;

/******************************************************************************/
void Renderer::addMaterial(std::shared_ptr<Material> material)
{
  /* Check if the material is transparent. */
  if(material->isTransparent())
  {
    /* Add it to the transparent material list. */
    fTransparentMaterials.push_back(material);
  }
  else
  {
    /* Add it to the opaque materials. */
    fOpaqueMaterials.push_back(material);
  }
}

/******************************************************************************/
void Renderer::frustumCull(std::shared_ptr<Camera> camera,
        std::vector<std::shared_ptr<Material>> & materials)
{
  /* Iterate through all the opaque materials. Since only meshes with materials
   * can be rendered, no point wasting time parsing the whole tree.*/
  for(auto material : materials)
  {
    /* A flag to indicate if atleast on mesh was visible. */
    bool matIsVisible = false;

//    /* Iterate through all the meshes in the material. */
//    for(auto mesh : material->meshes())
//    {
//      /* Update the mesh ref to calculate the visibility. */
//      matIsVisible |= frustumCull(camera, mesh);
//    }

    /* Mark the material as visible or invisible depending on whether atleast
     * one mesh was visible. */
    material->setVisible(camera, matIsVisible);
  }
}
/******************************************************************************/
void Renderer::occlusionCull(std::shared_ptr<Camera> camera,
                std::vector<std::shared_ptr<Material>> & opaqueMats,
                std::vector<std::shared_ptr<Material>> & transMats)
{
  /* Depth sort the opaque meshes. Object closer to the camera will likely be
   * infront of larger occluders, but will also likely occlude more of the
   * scene even if smaller. */

  /* Perform the occlusion renders. */

  /* Read the texture back. */

  /* Use SMID to detect the first diff. */

}

/******************************************************************************/
void Renderer::update()
{
  /* Iterate through the scene graph and update the world transform of
   * every node. */

  /* Iterate through all the cameras in the scene. */
  for(auto camera : fCameras)
  {
    /* Frustum cull the opaque meshes. */
    frustumCull(camera, fOpaqueMaterials);

    /* Frustum cull the transparent meshes. */
    frustumCull(camera, fTransparentMaterials);

    /* Occlusion cull the remaining visible meshes. */
    occlusionCull(camera, fOpaqueMaterials, fTransparentMaterials);
  }

  /* Depth sort the transparent faces and build the Trans Blocks queue for the
   * current camera. */

}

/******************************************************************************/
void Renderer::render()
{
  /* Iterate through all the cameras. */
  for(auto camera : fCameras)
  {
    /* Make the camera active. */
    //camera->activate();

    /* Iterate through all the opaque materials. */
    for(auto material : fOpaqueMaterials)
    {
      /* Check if anythging will be rendered with this material. If nothing
       * will be rendered, don't time making the state change. */
      if(!material->isVisible(camera))
      {
        /* Skip this material and process the next material. */
        continue;
      }

      /* Make the material active. */
      //material->activate();

      /* Iterate through all the meshes. */
//      for(auto mesh : material->meshes())
//      {
//        /* Check if the mesh is visible. */
//        if(mesh->isVisible(camera))
//        {
//          /* Activate the mesh. */
//          mesh->activate(camera);

//          /* Render the mesh using all it's indices. */
// //         mesh->render(camera);
//        }
//      }
    }

    /* Get the transparent block queue associated with the camera. */
    auto iter = fTransBlocks.find(camera);

    /* Check if there is anything transparent to render. */
    if(iter != fTransBlocks.end())
    {
      /* Iterate through all the transparent blocks. */
      for(auto transBlock : iter->second)
      {
        /* Make the material active. */
        //transBlock.fMaterial->activate();

        /* Activate the mesh. */
        transBlock.fMesh->activate(camera);

        /* Render the mesh only with the specific indices. */
        //transBlock.fMesh->render(transBlock.fIndices);
      }
    }
  }
}
