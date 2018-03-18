
find_package(glfw3 3.2 CONFIG REQUIRED PATH_SUFFIXES "/cmake/glfw3")

set(AnubisExample_SimpleRendering_SOURCES
  Main.cpp
  ../../Libraries/Glad/Source/glad.c
)

set(AnubisExample_SimpleRendering_HEADERS
  ../../Libraries/Glad/Include/glad/glad.h
  ../../Libraries/Glad/Include/KHR/khrplatform.h
)

include_directories(Include)

add_executable(AnubisExample_SimpleRendering ${AnubisExample_SimpleRendering_SOURCES}
  ${AnubisExample_SimpleRendering_HEADERS})

target_link_libraries(AnubisExample_SimpleRendering glfw AnubisGraphics AnubisMath
  AnubisCommon AnubisPhysics)
