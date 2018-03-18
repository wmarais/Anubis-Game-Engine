#ifndef ANUBIS_GRAPHICS_GL_STATE_HPP
#define ANUBIS_GRAPHICS_GL_STATE_HPP

#include "../Common/Misc.hpp"

namespace Anubis
{
  namespace Graphics
  {
    /***********************************************************************//**
     * A class to track and reduce the number of OpenGL state changes. It would
     * appear that even when OpenGL is allready in the correct state, simply
     * invoking the respective function will still cause a state change on the
     * video card and as such a stall. For this reason, this class track the
     * important state information and only invoke a state change when
     * absolutely required. This class can be extended and all state change
     * function can be overwritten.
     **************************************************************************/
    class GLState
    {
    protected:

      /** Indicate if the depth testing has been enabled or not. */
      bool fDepthTestEnabled;

      /*********************************************************************//**
       * Initialise the OpenGL state. This function can be overwritten to
       * change the initial OpenGL state.
       ************************************************************************/
      virtual void setInitialState();

    public:

      GLState();

      virtual ~GLState();

      virtual void setDepthTest(bool state);

    };
  }
}


#endif /* ANUBIS_GRAPHICS_GL_STATE_HPP */
