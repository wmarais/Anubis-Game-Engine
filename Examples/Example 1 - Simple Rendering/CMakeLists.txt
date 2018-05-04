
find_package(glfw3 3.2 CONFIG PATH_SUFFIXES "/cmake/glfw3")


set(AnubisExample_SimpleRendering_SOURCES
  Main.cpp
  ../../Libraries/Glad/Source/glad.c
)

set(AnubisExample_SimpleRendering_HEADERS
  ../../Libraries/Glad/Include/glad/glad.h
  ../../Libraries/Glad/Include/glad/khrplatform.h
)

include_directories(Include)

# Check if GLFW was found. If not, don't build the example.
if(glfw3_FOUND)
  add_executable(AnubisExample_SimpleRendering
    ${AnubisExample_SimpleRendering_SOURCES}
    ${AnubisExample_SimpleRendering_HEADERS})

  target_link_libraries(AnubisExample_SimpleRendering AnubisSimulation AnubisGraphics AnubisCommon
    AnubisMaths glfw dl)

  file(COPY "Font" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()
