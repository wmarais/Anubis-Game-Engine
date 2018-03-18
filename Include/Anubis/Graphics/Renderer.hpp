#ifndef ANUBIS_GRAPHICS_RENDERER_HPP
#define ANUBIS_GRAPHICS_RENDERER_HPP

#include "Material.hpp"
#include "Camera.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class Renderer
    {
      /*********************************************************************//**
       * The queue used for rendering transparent materials. The
       ************************************************************************/
      struct TransBlock
      {
        /*******************************************************************//**
         * The material associated with this block.
         **********************************************************************/
        std::shared_ptr<Material> fMaterial;

        /*******************************************************************//**
         * The mesh used for rendering.
         **********************************************************************/
        std::shared_ptr<MeshRef> fMesh;

        /*******************************************************************//**
         * The indices to render this block of faces.
         **********************************************************************/
        std::vector<uint16_t> fIndices;
      };

      /*********************************************************************//**
       * The depth sorted queue of transparent faces to render.
       ************************************************************************/
      std::map<std::shared_ptr<Camera>, std::vector<TransBlock>> fTransBlocks;

      /*********************************************************************//**
       * The list of opaque materials used for rendering. These materials do not
       * require the faces to be sorted before rendering.
       ************************************************************************/
      std::vector<std::shared_ptr<Material>> fOpaqueMaterials;

      /*********************************************************************//**
       * The list of transparent materials. These materials required that the
       * faces are sorted before rendering.
       ************************************************************************/
      std::vector<std::shared_ptr<Material>> fTransparentMaterials;

      /*********************************************************************//**
       * The list of cameras to render.
       ************************************************************************/
      std::vector<std::shared_ptr<Camera>> fCameras;

      /*********************************************************************//**
       * Check whether the mesh is visible within the camera's frustum.
       *
       * @return True if the mesh is visible in the frustum, else false.
       ************************************************************************/
      static bool frustumCull(std::shared_ptr<Camera> camera,
                       std::shared_ptr<MeshRef> mesh) { return true; }

      /*********************************************************************//**
       * Calculate the visibility of the renderable meshes.
       ************************************************************************/
      static void frustumCull(std::shared_ptr<Camera> camera,
                     std::vector<std::shared_ptr<Material>> & materials);

      static void occlusionCull(std::shared_ptr<Camera> camera,
                      std::vector<std::shared_ptr<Material>> & opaqueMats,
                      std::vector<std::shared_ptr<Material>> & transMats);

    public:

      void addMaterial(std::shared_ptr<Material> material);

      /*********************************************************************//**
       * Update the scene baking out world transforms, performing visibility
       * calculations and depth sorting transparent faces.
       ************************************************************************/
      void update();

      /*********************************************************************//**
       * Render the scene, first the opaque meshes then the transparent ones.
       ************************************************************************/
      void render();

    };
  }
}

#endif /* ANUBIS_GRAPHICS_RENDERER_HPP */
