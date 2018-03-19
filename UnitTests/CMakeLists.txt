# Find the Google Test package.
find_package(gtest)

include_directories(${GTEST_INCLUDE_DIRS})

# All the header files for the unit tests.
set(AnubisUnitTest_HEADERS
  Include/FloatTests.hpp
  Include/Matrix4fTests.hpp
  Include/PhysicsTests.hpp
  Include/Vector4fTests.hpp
)

# All the source files for the unit tests.
set(AnubisUnitTest_SOURCES
  Source/Main.cpp
)

# Build the test executable.
add_executable(AnubisUnitTest ${AnubisUnitTest_HEADERS}
  ${AnubisUnitTest_SOURCES})

# Link to all the required libraries.
target_link_libraries(AnubisUnitTest Anubis ${GTEST_LIBRARIES})
