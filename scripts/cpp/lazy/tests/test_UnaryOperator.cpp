#include "main.h"

#include "lazy/Random.h"
#include "lazy/UnaryOperator.h"


static auto test_negate = make_test(
  "Test Unary Negate",
  [] (std::size_t const &inSeed) {
    constexpr auto positive = lazy::Random<1, 100>();
    constexpr auto negative = -positive;
    // Set the random seed so the testcase is reproduceable
    lazy::random::engine = std::default_random_engine(inSeed);
    int positive_value = positive();
    // Set the random seed so the second draw is the same as the first
    lazy::random::engine = std::default_random_engine(inSeed);
    int negative_value = negative();
    RC_ASSERT(-positive_value == negative_value);
  }
);
