#ifndef ANUBIS_GRAPHICS_SHADER_PROGRAM_HPP
#define ANUBIS_GRAPHICS_SHADER_PROGRAM_HPP

#include "Shader.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class ShaderProgram final
    {
    public:
      ShaderProgram(std::vector<std::shared_ptr<Shader>> & shaders);

      /*********************************************************************//**
       * Destroy the resources that was allocated for the shader program.
       ************************************************************************/
      ~ShaderProgram();

      /*********************************************************************//**
       * Make the shader program active for rendering.
       ************************************************************************/
      void activate();

      /*********************************************************************//**
       * Deactivate the current shader program. It's not necisary to invoke this
       * before making another program active.
       ************************************************************************/
      void deactivate();

    private:
      /*************************************************************************
       * Delete the copy constructor and assignment operator to prevent shallow
       * copies.
       ************************************************************************/
      ShaderProgram(const ShaderProgram & cp) = delete;
      ShaderProgram & operator = (const ShaderProgram & rhs) = delete;

      /*********************************************************************//**
       * The OpenGL id of the shader program.
       ************************************************************************/
      uint32_t fID;
    };
  }
}

#endif /* ANUBIS_GRAPHICS_SHADER_PROGRAM_HPP */
