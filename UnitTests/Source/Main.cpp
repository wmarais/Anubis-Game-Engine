#include "../Include/FloatTests.hpp"
#include "../Include/Vector4fTests.hpp"
#include "../Include/Matrix4fTests.hpp"
#include "../Include/PhysicsTests.hpp"

int main(int argc, char * argv[])
{
  /*  Initialise the testing framework. */
  ::testing::InitGoogleTest(&argc, argv);

  /* Run all the unit tests. */
  return RUN_ALL_TESTS();
}
