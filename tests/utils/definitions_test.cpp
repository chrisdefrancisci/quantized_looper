#include <gtest/gtest.h>

#include <quantized_looper/utils/definitions.hpp>

TEST(DefinitionsTest, Scale)
{
    using namespace QuantizedLooper;
    // Min-to-min conversion
    EXPECT_EQ(scale_input(analogMin), computationMin);
    EXPECT_EQ(scale_output(computationMin), analogMin);

    // Max-to-max conversion
    EXPECT_EQ(scale_input(analogMax), computationMax);
    EXPECT_EQ(scale_output(computationMax), analogMax);

    // Mid-to-mid conversion
    EXPECT_EQ(scale_input((analogMax - analogMin) / 2 + analogMin),
              (computationMax - computationMin) / 2.0f + computationMin);
    EXPECT_EQ(
      scale_output((computationMax - computationMin) / 2.0f + computationMin),
      (analogMax - analogMin) / 2 + analogMin);
}