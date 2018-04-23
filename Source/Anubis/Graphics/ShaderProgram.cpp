#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Graphics/ShaderProgram.hpp"

using namespace Anubis::Graphics;

/******************************************************************************/
ShaderProgram::ShaderProgram(std::vector<std::shared_ptr<Shader>> &shaders)
{
  /* Create a shader program. */
  fID = glCreateProgram();

  /* Attach all the shaders to the program. */
  for(auto shader : shaders)
  {
    glAttachShader(fID, shader->id());
  }

  /* Link the shader program. */
  glLinkProgram(fID);

  /* Get the compiled status of the shader. */
  GLint isLinked = 0;
  glGetShaderiv(fID, GL_LINK_STATUS, &isLinked);

  /* Check if compilation failed. */
  if(isLinked == GL_FALSE)
  {
    /* Get the log information from the compilation operation. */
    GLint maxLength = 0;
    glGetShaderiv(fID, GL_INFO_LOG_LENGTH, &maxLength);

    /* Create the required string to store the log. */
    std::string logMessage;
    logMessage.resize(maxLength);

    /* Read the string. */
    glGetProgramInfoLog(fID, maxLength, &maxLength, (GLchar*)(logMessage.data()));

    /* Resize the string in case it has garbage at the end. */
    logMessage.resize(maxLength);

    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to link the shader program, "
      "because: " << logMessage.data() << ", Log Length: " << maxLength);
  }
}

/******************************************************************************/
ShaderProgram::~ShaderProgram()
{
  /* Check if the shader program is valid. */
  if(fID != 0)
  {
    /* Delete the shader. */
    glDeleteProgram(fID);
  }
}

/******************************************************************************/
void ShaderProgram::activate()
{
  /* Make the program active. */
  glUseProgram(fID);
}

/******************************************************************************/
void ShaderProgram::deactivate()
{
  /* Select the null program. */
  glUseProgram(0);
}
