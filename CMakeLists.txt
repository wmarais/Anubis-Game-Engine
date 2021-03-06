cmake_minimum_required(VERSION 3.8)
project(Anubis)

# Set the include directory for the generated config.hpp file.
include_directories("${CMAKE_BINARY_DIR}/Include/Anubis")
include_directories("${CMAKE_BINARY_DIR}/Include/Anubis/Common")

# Include optional cmake modules.
include(TestBigEndian)
include(CMakeDependentOption)
include(HostTests/TestSSE.cmake)

################################################################################
# FIND PACKAGES
################################################################################
find_package(Doxygen)

find_library(ANUBIS_HAS_RPCRT4 rpcrt4)
find_library(ANUBIS_HAS_WINSOCK2 ws2_32)
find_library(ANUBIS_HAS_PHYSFS physfs)


find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

################################################################################
# HOST DETECTION
################################################################################

# Check if the host is big or little endian.
test_big_endian(ANUBIS_HOST_IS_BIG_ENDIAN)
if(NUBIS_HOST_IS_BIG_ENDIAN)
  set(ANUBIS_CONF_ENDIANESS "ANUBIS_BIG_ENDIAN")
else()
  set(ANUBIS_CONF_ENDIANESS "ANUBIS_LITTLE_ENDIAN")
endif()

# Check the OS being compiled for.
if(WIN32)
  set(ANUBIS_CONF_OS "ANUBIS_OS_WINDOWS")
elseif(APPLE)
  set(ANUBIS_CONF_OS "ANUBIS_OS_OSX")
elseif(UNIX)
  set(ANUBIS_CONF_OS "ANUBIS_OS_UNIX")
endif()

# Check if the C++17 aligned_alloc() function exist.
include(CheckFunctionExists)
check_function_exists("aligned_alloc" ANUBIS_HAS_STD_ALIGNED_ALLOC)

if(ANUBIS_HAS_STD_ALIGNED_ALLOC)
    message("aligned_alloc exist!")
else()
    message("aligned_alloc does not exist!")
endif()

################################################################################
# USER CONFIGURABLE BUILD OPTIONS
################################################################################

# Check if data-packs only must be loaded, instead of directories and data packs.
option(ANUBIS_OPT_LOAD_DATA_PACK_ONLY
  "Only load data packs and not directories." OFF)

if(ANUBIS_OPT_LOAD_DATA_PACK_ONLY)
  set(ANUBIS_CONF_LOAD_DATA_PACK_ONLY "true")
else()
  set(ANUBIS_CONF_LOAD_DATA_PACK_ONLY "false")
endif()

# Check if the maths library must be built.
option(ANUBIS_BUILD_MATHS "Build the mathematics library." ON)

# Check if the network library must be built.
option(ANUBIS_BUILD_NETWORKING "Build the networking library." ON)

# Check if the physics library must be built.
cmake_dependent_option(ANUBIS_BUILD_PHYSICS "Build the physics library." ON
  "ANUBIS_BUILD_MATHS" OFF)

cmake_dependent_option(ANUBIS_BUILD_SIMULATION "Build the simulation library."
  ON "ANUBIS_BUILD_MATHS" OFF)

# Check if the graphics library must be built.
cmake_dependent_option(ANUBIS_BUILD_GRAPHICS "Build the graphics library." ON
  "ANUBIS_BUILD_MATHS" OFF)

# Check if the examples must be built.
cmake_dependent_option(ANUBIS_BUILD_EXAMPLES "Build the example programs." ON
  "ANUBIS_BUILD_MATHS;ANUBIS_BUILD_PHYSICS;ANUBIS_BUILD_GRAPHICS" OFF)

# Check if the unit tests must be built.
cmake_dependent_option(ANUBIS_BUILD_UNIT_TESTS "Build the unit tests." ON
  "ANUBIS_BUILD_MATHS;ANUBIS_BUILD_PHYSICS;ANUBIS_BUILD_GRAPHICS" OFF)

# Check if doxygen must be generated.
cmake_dependent_option(ANUBIS_GENERATE_DOXYGEN "Generate doxygen documentation."
  ON "DOXYGEN_FOUND" OFF)

# Check for SSE support.
AnubisHasSSE()
cmake_dependent_option(ANUBIS_ENABLE_SSE "Build with SSE4.1 support."
  ${ANUBIS_SSE_CAN_RUN} "ANUBIS_SSE_CAN_COMPILE" OFF)

# Check the OS.
if(WIN32)
  set(ANUBIS_OS_WINDOWS True)
endif()

################################################################################
# CLANG COMPILER OPTIONS
################################################################################
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
endif()

################################################################################
# GNU / G++ COMPILER OPTIONS
################################################################################
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Check if SSE4.1 should be enabled.
  if(ANUBIS_ENABLE_SSE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -pthread")
    set(ANUBIS_CONF_SIMD_VERSION "ANUBIS_SIMD_SSE4")
  else()
    set(ANUBIS_CONF_SIMD_VERSION "ANUBIS_SIMD_NONE")
  endif()

  # Enable all compiler warnings and C++ 17.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

  # Check if MINGW is used for compiling.
  if(MINGW)
      set(ANUBIS_CONF_COMPILER "ANUBIS_COMPILER_MINGW")
  else()
      set(ANUBIS_CONF_COMPILER "ANUBIS_COMPILER_GXX")
  endif()
endif()

################################################################################
# INTEL COMPILER OPTIONS
################################################################################
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
endif()

################################################################################
# VISUAL C COMPILER OPTIONS
################################################################################
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
endif()

################################################################################
# MATHS SOURCE CODE
################################################################################
set(AnubisCommon_HEADERS
  Include/Anubis/Common.hpp
  Include/Anubis/Common/Algorithms.hpp
  Include/Anubis/Common/Barrier.hpp
  Include/Anubis/Common/CircularQueue.hpp
  Include/Anubis/Common/DataPack.hpp
  Include/Anubis/Common/File.hpp
  Include/Anubis/Common/Float.hpp
  Include/Anubis/Common/IdentObj.hpp
  Include/Anubis/Common/Library.hpp
  Include/Anubis/Common/Log.hpp
  Include/Anubis/Common/Memory.hpp
  Include/Anubis/Common/Misc.hpp
  Include/Anubis/Common/SubObj.hpp
  Include/Anubis/Common/System.hpp
  Include/Anubis/Common/UUID.hpp

  Include/Anubis/Common/System/SocketWrapper.hpp
)

set(AnubisCommon_SOURCES
  Source/Anubis/Common/Algorithms.cpp
  Source/Anubis/Common/Barrier.cpp
  Source/Anubis/Common/DataPack.cpp
  Source/Anubis/Common/File.cpp
  Source/Anubis/Common/Float.cpp
  Source/Anubis/Common/Library.cpp
  Source/Anubis/Common/Log.cpp
  Source/Anubis/Common/UUID.cpp

  Source/Anubis/Common/System/SocketWrapper.cpp
)

# The Simulation Library header and source files.
if(ANUBIS_BUILD_SIMULATION)
  set(AnubisSimulation_HEADERS
    Include/Anubis/Simulation/Context.hpp
    Include/Anubis/Simulation/Client.hpp
    Include/Anubis/Simulation/Server.hpp
  )

  set(AnubisSimulation_SOURCES
    Source/Anubis/Simulation/Context.cpp
    Source/Anubis/Simulation/Client.cpp
    Source/Anubis/Simulation/Server.cpp
  )
endif()

# The Graphics Library header and source files.
if(ANUBIS_BUILD_GRAPHICS)
  set(AnubisGraphics_HEADERS
    Include/Anubis/Graphics.hpp
    Include/Anubis/Graphics/AttributeArray.hpp
    Include/Anubis/Graphics/BufferObject.hpp
    Include/Anubis/Graphics/Camera.hpp
    Include/Anubis/Graphics/Colour.hpp
    Include/Anubis/Graphics/GLState.hpp
    Include/Anubis/Graphics/LODSet.hpp
    Include/Anubis/Graphics/MixedAttributeArray.hpp
    Include/Anubis/Graphics/PixelMap.hpp
    Include/Anubis/Graphics/Material.hpp
    Include/Anubis/Graphics/Mesh.hpp
    Include/Anubis/Graphics/MeshGenerator.hpp
    Include/Anubis/Graphics/MeshRef.hpp
    Include/Anubis/Graphics/Renderer.hpp
    Include/Anubis/Graphics/Shader.hpp
    Include/Anubis/Graphics/ShaderProgram.hpp
    Include/Anubis/Graphics/TextRenderer.hpp
    Include/Anubis/Graphics/Texture.hpp
    Include/Anubis/Graphics/Texture2D.hpp
  )

  set(AnubisGraphics_SOURCES
    Source/Anubis/Graphics/AttributeArray.cpp
    Source/Anubis/Graphics/BufferObject.cpp
    Source/Anubis/Graphics/Camera.cpp
    Source/Anubis/Graphics/Colour.cpp
    Source/Anubis/Graphics/GLState.cpp
    Source/Anubis/Graphics/MixedAttributeArray.cpp
    Source/Anubis/Graphics/PixelMap.cpp
    Source/Anubis/Graphics/Material.cpp
    Source/Anubis/Graphics/Mesh.cpp
    Source/Anubis/Graphics/MeshGenerator.cpp
    Source/Anubis/Graphics/Renderer.cpp
    Source/Anubis/Graphics/Shader.cpp
    Source/Anubis/Graphics/ShaderProgram.cpp
    Source/Anubis/Graphics/TextRenderer.cpp
    Source/Anubis/Graphics/Texture.cpp
    Source/Anubis/Graphics/Texture2D.cpp
  )
endif()

# The Mathematics Library header and source files.
if(ANUBIS_BUILD_MATHS)
  set(AnubisMath_HEADERS
    Include/Anubis/Math.hpp
    Include/Anubis/Math/Quaternion.hpp
    Include/Anubis/Math/Vector2f.hpp
    Include/Anubis/Math/Vector4f.hpp
    Include/Anubis/Math/Matrix4f.hpp
    Include/Anubis/Math/Ray.hpp
  )

  set(AnubisMath_SOURCES
    Source/Anubis/Math/Matrix4f.cpp
    Source/Anubis/Math/Quaternion.cpp
    Source/Anubis/Math/Vector4f.cpp
  )
endif()

# The Network Library header and source files.
if(ANUBIS_BUILD_NETWORKING)
  set(AnubisNetwork_HEADERS
    Include/Anubis/Networking.hpp
    Include/Anubis/Networking/IPEndPoint.hpp
    Include/Anubis/Networking/Socket.hpp
    Include/Anubis/Networking/TCPServer.hpp
    Include/Anubis/Networking/UDPServer.hpp
  )

  set(AnubisNetwork_SOURCES
    Source/Anubis/Networking/IPEndPoint.cpp
    Source/Anubis/Networking/Socket.cpp
    Source/Anubis/Networking/TCPServer.cpp
    Source/Anubis/Networking/UDPServer.cpp
  )
endif()

# The Physics Library header and source files.
if(ANUBIS_BUILD_PHYSICS)
  set(AnubisPhysics_HEADERS
    Include/Anubis/Physics.hpp
    Include/Anubis/Physics/BoundingBox.hpp
    Include/Anubis/Physics/BoundingMesh.hpp
    Include/Anubis/Physics/BoundingRect.hpp
    Include/Anubis/Physics/BoundingSphere.hpp
    Include/Anubis/Physics/BoundingVolume.hpp
    Include/Anubis/Physics/CameraNode.hpp
    Include/Anubis/Physics/PhysicsContext.hpp
    Include/Anubis/Physics/Scene.hpp
    Include/Anubis/Physics/TaskPool.hpp
  )

  set(AnubisPhysics_SOURCES
    Source/Anubis/Physics/BoundingBox.cpp
    Source/Anubis/Physics/BoundingMesh.cpp
    Source/Anubis/Physics/BoundingSphere.cpp
    Source/Anubis/Physics/BoundingVolume.cpp
    Source/Anubis/Physics/PhysicsContext.cpp
    Source/Anubis/Physics/Scene.cpp
    Source/Anubis/Physics/TaskPool.cpp
  )
endif()

################################################################################
# GENERATE CONFIG HEADER
################################################################################
configure_file(Config.hpp.in "Include/Anubis/Common/Config.hpp")

################################################################################
# BUILD SELECTED ITEMS
################################################################################
# Builds the common library.
add_library(AnubisCommon STATIC ${AnubisCommon_SOURCES} ${AnubisCommon_HEADERS})
target_link_libraries(AnubisCommon Boost::filesystem)

if(ANUBIS_BUILD_MATHS)
  add_library(AnubisMaths STATIC ${AnubisMath_SOURCES} ${AnubisMath_HEADERS})
endif()

if(ANUBIS_BUILD_PHYSICS)
  add_library(AnubisPhysics STATIC ${AnubisPhysics_SOURCES}
    ${AnubisPhysics_HEADERS})
endif()

if(ANUBIS_BUILD_GRAPHICS)
  add_library(AnubisGraphics STATIC ${AnubisGraphics_SOURCES}
    ${AnubisGraphics_HEADERS})

    target_link_libraries(AnubisGraphics ${FREETYPE_LIBRARY})
endif()

if(ANUBIS_BUILD_NETWORKING)
  add_library(AnubisNetwork STATIC ${AnubisNetwork_SOURCES}
    ${AnubisNetwork_HEADERS})

  if(ANUBIS_HAS_WINSOCK2)
    target_link_libraries(AnubisNetwork ${ANUBIS_HAS_WINSOCK2})
  endif()
endif()

if(ANUBIS_BUILD_SIMULATION)
  add_library(AnubisSimulation STATIC ${AnubisSimulation_SOURCES}
    ${AnubisSimulation_HEADERS})
endif()


if(ANUBIS_HAS_RPCRT4)
  target_link_libraries(AnubisCommon ${ANUBIS_HAS_RPCRT4})
endif()

if(ANUBIS_HAS_PHYSFS)
  target_link_libraries(AnubisCommon ${ANUBIS_HAS_PHYSFS})
endif()

# Build the examples.
if(ANUBIS_BUILD_EXAMPLES)
  add_subdirectory("Examples/Example 1 - Simple Rendering")
  add_subdirectory("Examples/Example 2 - Simple TCP Comms")
  add_subdirectory("Examples/Example 3 - Simple UDP Comms")
endif()

# Build the unit tests.
if(ANUBIS_BUILD_UNIT_TESTS)
  add_subdirectory(UnitTests)
endif()

# Generate the doxygen docs.
if(ANUBIS_GENERATE_DOXYGEN)
  # Generate the doxygen config.
  configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)

  # Generate the documentation.
  add_custom_target(AnubisDoxygen ALL
    ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()

################################################################################
# PRINT A SUMMARY
################################################################################
message("#######################")
message("Building:")

if(ANUBIS_BUILD_GRAPHICS)
  message("    -> Graphics")
endif()

if(ANUBIS_BUILD_MATHS)
  message("    -> Maths")
endif()

if(ANUBIS_BUILD_EXAMPLES)
  message("    -> Examples")
endif()

if(ANUBIS_BUILD_UNIT_TESTS)
  message("    -> Unit Tests")
endif()

if(ANUBIS_GENERATE_DOXYGEN)
  message("    -> Doxygen (HTML) Docs")
endif()

message("Skipping:")

if(NOT ANUBIS_BUILD_GRAPHICS)
  message("    -> Graphics")
endif()

if(NOT ANUBIS_BUILD_MATHS)
  message("    -> Maths")
endif()

if(NOT ANUBIS_BUILD_EXAMPLES)
  message("    -> Examples")
endif()

if(NOT ANUBIS_BUILD_UNIT_TESTS)
  message("    -> Unit Tests")
endif()

if(NOT ANUBIS_GENERATE_DOXYGEN)
  message("    -> Doxygen (HTML) Docs")
endif()
message("#######################")
