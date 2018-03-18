#ifndef ANUBIS_GRAPHICS_MATERIAL_HPP
#define ANUBIS_GRAPHICS_MATERIAL_HPP

#include "Camera.hpp"
#include "MeshRef.hpp"
#include "ShaderProgram.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class Material
    {
      /** The material currently used for rendering. */
      static Material * fActiveMaterial;

      /*********************************************************************//**
       * Indicate the transparency of the material. 1 = Completely Opaque, 0 =
       * Completely Transparent.
       ************************************************************************/
      float fOpacity;



      std::map<std::shared_ptr<Camera>, bool> fVisibility;

      /*********************************************************************//**
       * The shader program used to render the graphics for this material.
       ************************************************************************/
      std::shared_ptr<ShaderProgram> fShaderProgram;

      /*********************************************************************//**
       * The list of meshes associated with this material.
       ************************************************************************/
      std::vector<std::shared_ptr<MeshRef>> fMeshes;

      /*************************************************************************
       * Delete the copy constructor and the assignment operator to prevent
       * shallow copies of the material.
       ************************************************************************/
      Material(const Material & cp) = delete;
      Material & operator = (const Material & rhs) = delete;

    public:
      /*********************************************************************//**
       * Create a new material
       ************************************************************************/
      Material(std::shared_ptr<ShaderProgram> prog);

      /*********************************************************************//**
       * Check all the associated meshes to see if any are visible for the
       * particular camera.
       ************************************************************************/
      void update(std::shared_ptr<Camera> camera);

      /*********************************************************************//**
       * Indicate if the material is transparent or not. Use this to determine
       * whether the associated meshes and faces must be depth sorted before
       * being rendered.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool isTransparent() const
      {
        /* Check if the material is transparent or not. */
        return fOpacity != 1.0f;
      }

      /*********************************************************************//**
       * Indicate if any of the meshes attached to the material is visible in
       * the next frame render.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool isVisible(std::shared_ptr<Camera> camera)
      {
        return true;
      }

      ANUBIS_FORCE_INLINE void setVisible(std::shared_ptr<Camera> camera,
                                          bool isVisible)
      {
        fVisibility[camera] = isVisible;
      }

      std::vector<std::shared_ptr<MeshRef>> meshRefs() const
      {
        return fMeshes;
      }

      void activate(const Camera * camera);
    };
  }
}

#endif /* ANUBIS_GRAPHICS_MATERIAL_HPP */
