#ifndef ANUBIS_GRAPHICS_SHADER_HPP
#define ANUBIS_GRAPHICS_SHADER_HPP

#include "../Common.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class Shader final
    {
      /** The number of supported shader types. */
      static const size_t kTypesCount = 6;

      /** The lookup table to convert to OpenGL shader types. */
      static const uint32_t kGLTypes[kTypesCount];

      /** The strings that describe each shader. This is simply a name that
       * matches the Shader::Types enum, and is used for printing sensible
       * debug message.*/
      static const char * kTypeStrs[kTypesCount];

      /** The OpenGL ID of the shader. */
      uint32_t fID;

      /*********************************************************************//**
       * Compile the shader and ensure that it has been compiled properly by
       * checking the compilation result and checking error log messages.
       ************************************************************************/
      void compileAndVerify();

    public:
      /*********************************************************************//**
       * The list of supported shader types. Subject to the OpenGL version, not
       * all shader types may be available.
       ************************************************************************/
      enum class Types : uint8_t
      {
        Vertex = 0,
        Fragment,
        Compute,
        Geometry,
        TessControl,
        TessEvaluation
      };

      /*********************************************************************//**
       * Create a new shader. This will compile the shader.
       *
       * @param src   The source code of the shader.
       * @param type  The shader type.=
       ************************************************************************/
      Shader(Types type, std::initializer_list<std::string> src);

      /*********************************************************************//**
       * @brief Shader
       *
       * @param path  The path to the shader source code
       * @param src
       * @param type
       */
      Shader(const std::string & path, const std::string & src, Types type);

      /*********************************************************************//**
       * Destroy the shader.
       ************************************************************************/
      ~Shader();

      /*********************************************************************//**
       * Return the OpenGL id of the shader object.
       ************************************************************************/
      uint32_t id();

    private:
      /*************************************************************************
       * Delete the copy constructor and the assignment operator to prevent
       * shallow copies of the object.
       ************************************************************************/
      Shader(const Shader & cp) = delete;
      Shader & operator = (const Shader & rhs) = delete;

      /*********************************************************************//**
       * The number of diffirent shader types that are supported.
       ************************************************************************/
      static const size_t kShaderTypeCount = 6;
    };
  }
}

#endif // ANUBIS_GRAPHICS_SHADER_HPP
