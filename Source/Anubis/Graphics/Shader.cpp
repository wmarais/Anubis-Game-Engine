#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Graphics/Shader.hpp"

using namespace Anubis::Graphics;

/******************************************************************************/
const char * Shader::kTypeStrs[kShaderTypeCount]
{
  "Vertex",
  "Fragment",
  "Compute",
  "Geometry",
  "TessControl",
  "TessEvaluation"
};

/******************************************************************************/
Shader::Shader(Types type, std::initializer_list<std::string> src)
{
  /* Set the OpenGL type. */
  switch(type)
  {
    #ifdef GL_VERTEX_SHADER
      case Types::Vertex: fGLType = GL_VERTEX_SHADER; break;
    #endif /* GL_VERTEX_SHADER */

    #ifdef GL_FRAGMENT_SHADER
      case Types::Fragment: fGLType = GL_FRAGMENT_SHADER; break;
    #endif /* GL_FRAGMENT_SHADER */

    #ifdef GL_COMPUTE_SHADER
      case Types::Compute: fGLType = GL_COMPUTE_SHADER; break;
    #endif /* GL_COMPUTE_SHADER */

    #ifdef GL_GEOMETRY_SHADER
      case Types::Geometry: fGLType = GL_GEOMETRY_SHADER; break;
    #endif /* GL_GEOMETRY_SHADER */

    #ifdef GL_TESS_EVALUATION_SHADER
      case Types::TessEvaluation5: fGLType = GL_TESS_EVALUATION_SHADER; break;
    #endif /* GL_TESS_EVALUATION_SHADER */

    #ifdef GL_TESS_CONTROL_SHADER
      case Types::TessControl: fGLType = GL_TESS_CONTROL_SHADER; break;
    #endif /* GL_TESS_CONTROL_SHADER */

    default:
      ANUBIS_THROW_RUNTIME_EXCEPTION("The specified shader type (" <<
        kTypeStrs[static_cast<size_t>(type)] << ") is not availble in the " <<
        "current version of OpenGL.");
    break;
  }

  /* Create the shader item. */
  fID = glCreateShader(fGLType);

  /* Check if the shader was created. */
  if(fID == 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to create shader (" <<
      kTypeStrs[static_cast<size_t>(type)] << ").");
  }

  /* The number of sources. */
  int32_t count = static_cast<int32_t>(src.size());

  /* The char pointers to pass in. */
  std::vector<const GLchar*> charPtrs(count);

  /* The lengths of each source string. */
  std::vector<GLint> length(count);

  /* Build the list of lengths and the strings. */
  for(auto str : src)
  {
    charPtrs.push_back(static_cast<const GLchar*>(str.c_str()));
    length.push_back(str.length());
  }

  /* Set the shader source. */
  glShaderSource(fID, count, charPtrs.data(), length.data());

  /* Compile the shader. */
  glCompileShader(fID);

  /* Get the compiled status of the shader. */
  GLint isCompiled = 0;
  glGetShaderiv(fID, GL_COMPILE_STATUS, &isCompiled);

  /* Check if compilation failed. */
  if(isCompiled == GL_FALSE)
  {
    /* Get the log information from the compilation operation. */
    GLint maxLength = 0;
    glGetShaderiv(fID, GL_INFO_LOG_LENGTH, &maxLength);

    /* Create the required string to store the log. */
    std::string logMessage;
    logMessage.resize(maxLength);

    /* Read the string. */
    glGetShaderInfoLog(fID, maxLength, &maxLength,
                       &logMessage[0]);

    /* Resize the string in case it has garbage at the end. */
    logMessage.resize(maxLength);

    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to compile shader!");
  }
}

/******************************************************************************/
Shader::~Shader()
{
  /* Check if the shader is valid. */
  if(fID != 0)
  {
    /* Delete the shader. */
    glDeleteShader(fID);
  }
}

/******************************************************************************/
uint32_t Shader::id()
{
  return fID;
}
