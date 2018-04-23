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
const uint32_t Shader::kGLTypes[kTypesCount] =
{
  GL_VERTEX_SHADER,
  GL_FRAGMENT_SHADER,
  #ifdef GL_COMPUTE_SHADER
    GL_COMPUTE_SHADER
  #else
    0
  #endif /* GL_COMPUTE_SHADER */
  ,
  GL_GEOMETRY_SHADER,

  #ifdef GL_TESS_EVALUATION_SHADER
    GL_TESS_EVALUATION_SHADER
  #else
    0
  #endif /* GL_TESS_EVALUATION_SHADER */
  ,
  #ifdef GL_TESS_CONTROL_SHADER
    GL_TESS_CONTROL_SHADER
  #else
    0
  #endif /* GL_TESS_CONTROL_SHADER */
};

/******************************************************************************/
Shader::Shader(const std::string &path, const std::string & src, Types type) :
  fID(0)
{
  /* Create the shader. */
  fID = glCreateShader(kGLTypes[static_cast<uint8_t>(type)]);

  /* Check if the shader was created. */
  if(fID == 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to create shader (" <<
      kTypeStrs[static_cast<size_t>(type)] << ").");
  }

  /* The length of the shader source. */
  GLint length = static_cast<GLint>(src.length());

  /* The pointer to the shader source. */
  const GLchar * source = static_cast<const GLchar*>(src.c_str());

  /* Set the shader source. */
  glShaderSource(fID, 1, &source, &length);

  /* Compile and verify the compilation result. */
  compileAndVerify();
}

/******************************************************************************/
Shader::Shader(Types type, std::initializer_list<std::string> src) : fID(0)
{
  /* Create the shader item. */
  fID = glCreateShader(kGLTypes[static_cast<uint8_t>(type)]);

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

  /* Compile and verify the compilation result. */
  compileAndVerify();
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
void Shader::compileAndVerify()
{
  /* Compile the shader. */
  glCompileShader(fID);

  /* Get the compiled status of the shader. */
  GLint isCompiled = 0;
  glGetShaderiv(fID, GL_COMPILE_STATUS, &isCompiled);

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

  /* Check if compilation failed. */
  if(isCompiled == GL_FALSE)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to compile shader because: "
      << logMessage.data());
  }
  else
  {
    /* Just log any messages. */
    ANUBIS_LOG_INFO("Shader Compilation Log: " << logMessage.data());
  }
}

/******************************************************************************/
uint32_t Shader::id()
{
  return fID;
}
