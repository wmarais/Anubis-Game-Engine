#include "../../Libraries/Glad/Include/glad/glad.h"
#include "GLFW/glfw3.h"

#include "../../Include/Anubis/Common.hpp"
#include "../../Include/Anubis/Graphics.hpp"
#include "../../Include/Anubis/Math.hpp"
#include "../../Include/Anubis/Physics.hpp"

using namespace Anubis::Common;
using namespace Anubis::Graphics;
using namespace Anubis::Math;
using namespace Anubis::Physics;

/* The physics scene object. */
std::unique_ptr<Scene> gScene;

/* The library of physics objects. */
std::vector<std::shared_ptr<const BoundingVolume>> gBoundingVolumesLib;

/* The library of renderable meshes. */
std::vector<std::shared_ptr<const MeshRef>> gMeshRefsLib;

/* The library of meshes. */
std::vector<std::shared_ptr<Mesh>> gMeshLibrary;

/* The camera that is used to render the scene. */
std::unique_ptr<Camera> gCamera;

/* The OpenGL state object. */
std::unique_ptr<GLState> gGLState;

/* The library of LODSets. */
std::unique_ptr<Library<LODSet>> gLODSetLibrary;



/******************************************************************************/
void gInitScene()
{
  /****************************************
   * STEP 1 - CREATE MESHES
   ***************************************/
  /* Create a simple rectangle mesh. */
  std::shared_ptr<Mesh> mesh = MeshGenerator::createRectangle(
        1.0f, 2.0f,
        0, 0,
        kXAxis4f,
        kYAxis4f,
        Vector4f::makePosition(-0.5f, -0.5f, 0.0f),
        false, false, false);

  /* Add the mesh to the mesh library. */
  gMeshLibrary.push_back(mesh);

  /* Create a buffer object to render the mesh with.*/
  std::shared_ptr<BufferObject> buffObj =
      std::make_shared<BufferObject>(gMeshLibrary);

  /****************************************
   * STEP 2 - CREATE LOD SETS
   ***************************************/
  /* Create a LOD set with just the one mesh. Since there is only one mesh in
   * in the set, the lod level will always be calculated as this mesh, thus the
   * range can be set to anything, i.e. 0.0f is fine. */
  std::shared_ptr<LODSet> lodSet =
      std::make_shared<LODSet>(LODSet::Level(0.0f, mesh));


  /****************************************
   * STEP 3 - CREATE SCENE
   ***************************************/

  /* Anubis usesa concept of Identifiable Objects (IdentObj) and Sub Objects
   * (SubObj) to establish relationships between the physics, graphics, ai, etc
   * modules. This allow the module to effectively operate without knowlege of
   * the other. At it's core an identifiable object is simply and object that
   * a UUID and a subobject*/



  /* Create a camera. The Camera object is special in the sense that there is
   * both a Physic (CameraNode) and Graphics (Camera) variant. This is because
   * frustrum culling is per */

  //std::unique_ptr<Scene::Node> camera = std::make_unique()

  /* Creat the camera. */
  gCamera = std::make_unique<Camera>();

  /* Create an OpenGL state object. */
  gGLState = std::make_unique<GLState>();
}

/******************************************************************************/
void gUpdate()
{
}

/******************************************************************************/
void gRender()
{


}

/******************************************************************************/
static void gErrorCallback(int error, const char * description)
{
  std::cerr << "Error: (" << error << ") " << description << std::endl;
}

/******************************************************************************/
static void gKeyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods)
{
  /* Check if the escape key was pressed. */
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

/******************************************************************************/
int main(int argc, char * argv[])
{
  /* The pointer to the main window. */
  GLFWwindow * window;

  /* Set the error callback. */
  glfwSetErrorCallback(gErrorCallback);

  /* Initialise glfw. */
  if (!glfwInit())
  {
    exit(EXIT_FAILURE);
  }

  /* Specify to use OpenGL 3.2. */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  /* Create the application window. */
  window = glfwCreateWindow(800, 600, "Example 1 - Simple Rendering",
                            NULL, NULL);

  /* Check if the window was creted. */
  if (!window)
  {
    /* Terminate GLFW and exit with a failure message. */
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  /* Setup the keyboard callback. */
  glfwSetKeyCallback(window, gKeyCallback);

  /* Make the Windows GL context active. */
  glfwMakeContextCurrent(window);

  /* Load the OpenGL extensions required. */
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

  /* Disable vsync. */
  glfwSwapInterval(1);


  /* The physics context object. */
  std::unique_ptr<PhysicsContext> physicsContext =
      std::make_unique<PhysicsContext>();

  /* Initialise the scene. */
  gInitScene();

  /* While the windows is open, keep looping. */
  while (!glfwWindowShouldClose(window))
  {
    /* Update the scene. */
    gUpdate();

    /* Render the scene. */
    gRender();

    /* Swap the buffers. */
    glfwSwapBuffers(window);

    /* Poll for any input events. */
    glfwPollEvents();
  }

  /* Cleanly shutdown glfw. */
  glfwDestroyWindow(window);
  glfwTerminate();

  /* Return sucess to indicate the program executed error free. */
  return EXIT_SUCCESS;
}
