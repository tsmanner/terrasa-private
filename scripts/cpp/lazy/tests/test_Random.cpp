#include <list>
#include "main.h"

#include "lazy/Random.h"


template <int Min, int Max>
void checkRandom(std::size_t const &inSeed) {
  lazy::random::engine = std::default_random_engine(inSeed);
  auto expr = lazy::Random<Min, Max>();
  RC_ASSERT(expr() >= Min);
  RC_ASSERT(expr() <= Max);
}


static auto test_random = make_tests(
  make_test("Random<0,   0>", checkRandom<0,   0>),
  make_test("Random<0,   1>", checkRandom<0,   1>),
  make_test("Random<0, 100>", checkRandom<0, 100>),
  make_test("d4",             checkRandom<1,   4>),
  make_test("d6",             checkRandom<1,   6>),
  make_test("d8",             checkRandom<1,   8>),
  make_test("d10",            checkRandom<1,  10>),
  make_test("d12",            checkRandom<1,  12>),
  make_test("d20",            checkRandom<1,  20>)
);
