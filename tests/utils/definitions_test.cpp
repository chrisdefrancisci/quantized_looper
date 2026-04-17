#include <gtest/gtest.h>

#include <quantized_looper/utils/definitions.hpp>

TEST(DefinitionsTest, Scale)
{
    using namespace QuantizedLooper;
    // Min-to-min conversion
    EXPECT_EQ(scale(analogMin), computationMin);
    EXPECT_EQ(scale(computationMin), analogMin);

    // Max-to-max conversion
    EXPECT_EQ(scale(analogMax), computationMax);
    EXPECT_EQ(scale(computationMax), analogMax);

    // Mid-to-mid conversion
    EXPECT_EQ(scale((analogMax - analogMin) / 2 + analogMin),
              (computationMax - computationMin) / 2.0f + computationMin);
    EXPECT_EQ(scale((computationMax - computationMin) / 2.0f + computationMin),
              (analogMax - analogMin) / 2 + analogMin);
}