#include "../../../Include/Anubis/Graphics/GLState.hpp"
#include "../../../Include/Anubis/Common/System.hpp"

using namespace Anubis::Graphics;

/******************************************************************************/
GLState::GLState() : fDepthTestEnabled(true)
{
  /* Load the initial state. */
  setInitialState();
}

/******************************************************************************/
GLState::~GLState()
{

}

/******************************************************************************/
void GLState::setInitialState()
{
  /* By default enable depth testing. */
  fDepthTestEnabled = true;
  glEnable(GL_DEPTH_TEST);
}

/******************************************************************************/
void GLState::setDepthTest(bool state)
{
  /* Check if the state is allready correct. */
  if(fDepthTestEnabled == state)
  {
    /* Dont cause a state change, just return. */
    return;
  }

  /* Record the new state. */
  fDepthTestEnabled = state;

  /* Update the OpenGL state accordingly. */
  if(fDepthTestEnabled)
  {
    glEnable(GL_DEPTH_TEST);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }
}


