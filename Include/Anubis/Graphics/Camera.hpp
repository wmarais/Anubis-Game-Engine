#ifndef ANUBIS_GRAPHICS_CAMERA_HPP
#define ANUBIS_GRAPHICS_CAMERA_HPP

#include "../Common/Misc.hpp"
#include "../Common/SubObj.hpp"
#include "../Math/Vector4f.hpp"
#include "../Math/Matrix4f.hpp"
#include "../Physics/Scene.hpp"
#include "LODSet.hpp"
#include "../Common/Library.hpp"

namespace Anubis
{
  namespace Graphics
  {
    /* Forward declare the GLState class. */
    class GLState;

    /** Forward declare the class to avoide circular dependencies. */
    class Material;

    /** Forward declare the class to avoide circular dependencies. */
    class MeshRef;

    /***********************************************************************//**
     * The basic camera class used for rendering. Each camera has a rendering
     * queue used to only render the visible meshes in the correct order to
     * properly display opaque and transparent meshes.
     *
     * The default camera is implemented to act as a "Flight Camera", i.e. the
     * type used in flight simulators where strafing is relative to the current
     * camera transformation. To change the camera type, the strafeUp(),
     * strafeRight() and strafeForward() can be overloaded.
     **************************************************************************/
    class Camera : public Common::SubObj
    {
    protected:
      /*********************************************************************//**
       * A queue the stores the visible and properly sorted meshes that will be
       * rendered for the current frame.
       ************************************************************************/
      class RenderQueue
      {
        /*******************************************************************//**
         * A structure used for storing opaque mesh rendering information.
         **********************************************************************/
        struct OpaqueElement
        {
          /** The material to use for rendering the mesh refs. */
          std::shared_ptr<Material> fMaterial;

          /** The mesh refs to render, grouped by buffer object to reduce the
           * number of buffer object changes. */
          std::vector<std::shared_ptr<MeshRef>> fMeshRefs;
        };

        /*******************************************************************//**
         * A structure used for storing transparent mesh rendering information.
         **********************************************************************/
        struct TransparentElement
        {
          /** The material to use for rendering the mesh refs. */
          std::shared_ptr<Material> fMaterial;

          /** The mesh to render. */
          std::shared_ptr<MeshRef> fMeshRef;

          /** The indices to render. */
          std::vector<uint16_t> fIndices;
        };

        /** The opaqueue render queue. */
        std::vector<OpaqueElement> fOpaqueQueue;

        /** The transparent render queue. */
        std::vector<TransparentElement> fTransparentQueue;

      public:
        /*******************************************************************//**
         * Reset the state of the render queues. This should be done at the
         * start of each frame update. Rendering right after a reset will
         * produce no output since there will be nothign in the queues to
         * render.
         **********************************************************************/
        void reset();

        /*******************************************************************//**
         * Optimise the queues before rendering. For the opaque queue this
         * result n the queue being sorted such that meshes with the same buffer
         * object are rendered together. Currently no additional optimisations
         * exist for the transparent queue.
         **********************************************************************/
        void optimise();

        /*******************************************************************//**
         * Render the opaque meshes in the queue. This should always be called
         * before rendering the transparent meshes.
         *
         * @param camera  The camera that the scene is being rendered for.
         **********************************************************************/
        void renderOpaque(const Camera * camera);

        /*******************************************************************//**
         * Render the transparent meshes. This should always be called after
         * rendering all the opaque meshes.
         *
         * @param camera  The camera that the scene is being rendered for.
         **********************************************************************/
        void renderTransparent(const Camera * camera);
      };

      /** The render queue that will be used to render the scene for this
       * camera. */
      RenderQueue fRenderQueue;

      /** The position of the camera. */
      Anubis::Math::Vector4f fPosition;

      /** The x rotation angle. */
      float fXRotAngle;

      /** The y rotation angle. */
      float fYRotAngle;

      /** The z rotation angle. */
      float fZRotAngle;

      /*********************************************************************//**
       * Calculate the transformation matrix of the camera. Since diffirent
       * camera types required diffirent matrix multiplication orders, the
       * developer can overwrite this function to produce a custom
       * transformation matrix.
       *
       * @return  The transformation matrix of the camera.
       ************************************************************************/
      virtual Anubis::Math::Matrix4f calcTransform();

      /*void update(const Physics::Scene::Node * node,
                  const Common::Library<LODSet> & lodLibrary);*/

    public:

      /*********************************************************************//**
       * Set the position of the camera in world space.
       *
       * @param pos The position of the camera.
       ************************************************************************/
      void setPosition(const Anubis::Math::Vector4f & pos);

      /*********************************************************************//**
       * Make the camera look at a particular point in world space.
       *
       * @param pos The position to look at.
       ************************************************************************/
      void lookAt(const Anubis::Math::Vector4f & pos);

      /*********************************************************************//**
       * Update the rendering information (render queue) of the camera. This
       * must be done at the start of each frame. (Always update all the cameras
       * before rendering anything so that the video card does not stall.)
       *
       * @param scene The scene to render with this camera.
       ************************************************************************/
      void update(const Physics::Scene & scene,
                  const Common::Library<LODSet> &lodLibrary);

      /*********************************************************************//**
       * Render the scene, or rather the content of the render queue. This must
       * always be called after update().
       *
       * @param state The OpenGL state tracker object.
       ************************************************************************/
      void render(GLState * state);

      /*********************************************************************//**
       * Move the position of the camera up (positive step size) or down
       * (negative step size).
       *
       * @param step  The amount to move the camera by.
       ************************************************************************/
      virtual void strafeUp(float step);

      /*********************************************************************//**
       * Move the position of the camera right (positive step size) or left
       * (negative step size).
       *
       * @param step  The amount to move the camera by.
       ************************************************************************/
      virtual void strafeRight(float step);

      /*********************************************************************//**
       * Move the position of the camera forward (positive step size) or
       * backward (negative step size).
       *
       * @param step  The amout to move the camera by.
       ************************************************************************/
      virtual void strafeForward(float step);


      virtual void rotateAboutX(float step);

      virtual void rotateAboutY(float step);

      virtual void rotateAboutZ(float step);
    };
  }
}

#endif /* ANUBIS_GRAPHICS_CAMERA_HPP */
