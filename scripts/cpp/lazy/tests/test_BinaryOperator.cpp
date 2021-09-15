#include <sstream>
#include "main.h"

#include "lazy/Random.h"
#include "lazy/BinaryOperator.h"


static auto test_BinaryOperators = make_tests(
  make_test("Test plus",
    [] (std::size_t const &inSeed) {
      using lazy::Random;
      constexpr auto lhs = Random<1, 100>();
      constexpr auto rhs = Random<1, 100>();
      auto expr = lhs + rhs;
      // Set the random seed so the testcase is reproduceable
      lazy::random::engine = std::default_random_engine(inSeed);
      int lhs_value = lhs();
      int rhs_value = rhs();
      // Set the random seed so the second draw is the same as the first
      lazy::random::engine = std::default_random_engine(inSeed);
      int expr_value = expr();
      RC_ASSERT(expr_value == lhs_value + rhs_value);
      // Make sure streaming works correctly
      std::ostringstream ss;
      ss << expr;
      std::string expr_str = ss.str();
      ss.str("");
      ss << lhs << " + " << rhs;
      std::string expected_str = ss.str();
      RC_ASSERT(expr_str == expected_str);
    }
  ),
  make_test("Test minus",
    [] (std::size_t const &inSeed) {
      using lazy::Random;
      constexpr auto lhs = Random<1, 100>();
      constexpr auto rhs = Random<1, 100>();
      auto expr = lhs - rhs;
      // Set the random seed so the testcase is reproduceable
      lazy::random::engine = std::default_random_engine(inSeed);
      int lhs_value = lhs();
      int rhs_value = rhs();
      // Set the random seed so the second draw is the same as the first
      lazy::random::engine = std::default_random_engine(inSeed);
      int expr_value = expr();
      RC_ASSERT(expr_value == lhs_value - rhs_value);
      // Make sure streaming works correctly
      std::ostringstream ss;
      ss << expr;
      std::string expr_str = ss.str();
      ss.str("");
      ss << lhs << " - " << rhs;
      std::string expected_str = ss.str();
      RC_ASSERT(expr_str == expected_str);
    }
  ),
  make_test("Test multiplies",
    [] (std::size_t const &inSeed) {
      using lazy::Random;
      constexpr auto lhs = Random<1, 100>();
      constexpr auto rhs = Random<1, 100>();
      auto expr = lhs * rhs;
      // Set the random seed so the testcase is reproduceable
      lazy::random::engine = std::default_random_engine(inSeed);
      int lhs_value = lhs();
      int rhs_value = rhs();
      // Set the random seed so the second draw is the same as the first
      lazy::random::engine = std::default_random_engine(inSeed);
      int expr_value = expr();
      RC_ASSERT(expr_value == lhs_value * rhs_value);
      // Make sure streaming works correctly
      std::ostringstream ss;
      ss << expr;
      std::string expr_str = ss.str();
      ss.str("");
      ss << lhs << " * " << rhs;
      std::string expected_str = ss.str();
      RC_ASSERT(expr_str == expected_str);
    }
  ),
  make_test("Test divides",
    [] (std::size_t const &inSeed) {
      using lazy::Random;
      constexpr auto lhs = Random<1, 100>();
      constexpr auto rhs = Random<1, 100>();
      auto expr = lhs / rhs;
      // Set the random seed so the testcase is reproduceable
      lazy::random::engine = std::default_random_engine(inSeed);
      int lhs_value = lhs();
      int rhs_value = rhs();
      // Set the random seed so the second draw is the same as the first
      lazy::random::engine = std::default_random_engine(inSeed);
      int expr_value = expr();
      RC_ASSERT(expr_value == lhs_value / rhs_value);
      // Make sure streaming works correctly
      std::ostringstream ss;
      ss << expr;
      std::string expr_str = ss.str();
      ss.str("");
      ss << lhs << " / " << rhs;
      std::string expected_str = ss.str();
      RC_ASSERT(expr_str == expected_str);
    }
  ),
  make_test("Test modulus",
    [] (std::size_t const &inSeed) {
      using lazy::Random;
      constexpr auto lhs = Random<1, 100>();
      constexpr auto rhs = Random<1, 100>();
      auto expr = lhs % rhs;
      // Set the random seed so the testcase is reproduceable
      lazy::random::engine = std::default_random_engine(inSeed);
      int lhs_value = lhs();
      int rhs_value = rhs();
      // Set the random seed so the second draw is the same as the first
      lazy::random::engine = std::default_random_engine(inSeed);
      int expr_value = expr();
      RC_ASSERT(expr_value == lhs_value % rhs_value);
      // Make sure streaming works correctly
      std::ostringstream ss;
      ss << expr;
      std::string expr_str = ss.str();
      ss.str("");
      ss << lhs << " % " << rhs;
      std::string expected_str = ss.str();
      RC_ASSERT(expr_str == expected_str);
    }
  ),
  make_test("Test minimum",
    [] (std::size_t const &inSeed) {
      using lazy::Random;
      constexpr auto lhs = Random<1, 100>();
      constexpr auto rhs = Random<1, 100>();
      auto expr = lazy::min(lhs, rhs);
      // Set the random seed so the testcase is reproduceable
      lazy::random::engine = std::default_random_engine(inSeed);
      int lhs_value = lhs();
      int rhs_value = rhs();
      // Set the random seed so the second draw is the same as the first
      lazy::random::engine = std::default_random_engine(inSeed);
      int expr_value = expr();
      RC_ASSERT(expr_value == std::min(lhs_value, rhs_value));
      // Make sure streaming works correctly
      std::ostringstream ss;
      ss << expr;
      std::string expr_str = ss.str();
      ss.str("");
      ss << lhs << " min " << rhs;
      std::string expected_str = ss.str();
      RC_ASSERT(expr_str == expected_str);
    }
  ),
  make_test("Test maximum",
    [] (std::size_t const &inSeed) {
      using lazy::Random;
      constexpr auto lhs = Random<1, 100>();
      constexpr auto rhs = Random<1, 100>();
      auto expr = lazy::max(lhs, rhs);
      // Set the random seed so the testcase is reproduceable
      lazy::random::engine = std::default_random_engine(inSeed);
      int lhs_value = lhs();
      int rhs_value = rhs();
      // Set the random seed so the second draw is the same as the first
      lazy::random::engine = std::default_random_engine(inSeed);
      int expr_value = expr();
      RC_ASSERT(expr_value == std::max(lhs_value, rhs_value));
      // Make sure streaming works correctly
      std::ostringstream ss;
      ss << expr;
      std::string expr_str = ss.str();
      ss.str("");
      ss << lhs << " max " << rhs;
      std::string expected_str = ss.str();
      RC_ASSERT(expr_str == expected_str);
    }
  ),
  make_test("Test all binary operators",
    [] (std::size_t const &inSeed) {
      using lazy::Random;
      constexpr auto a = Random<1, 100>();
      constexpr auto b = Random<1, 100>();
      constexpr auto c = Random<1, 100>();
      constexpr auto d = Random<1, 100>();
      constexpr auto e = Random<1, 100>();
      constexpr auto f = Random<1, 100>();
      constexpr auto g = Random<1, 100>();
      constexpr auto h = Random<1, 100>();
      auto expr = lazy::max(lazy::min(a + (b - c) * d / e % f, g), h);
      // Set the random seed so the testcase is reproduceable
      lazy::random::engine = std::default_random_engine(inSeed);
      int a_value = a();
      int b_value = b();
      int c_value = c();
      int d_value = d();
      int e_value = e();
      int f_value = f();
      int g_value = g();
      int h_value = h();
      // Set the random seed so the second draw is the same as the first
      lazy::random::engine = std::default_random_engine(inSeed);
      int expr_value = expr();
      // Calculate the expected result
      int expected_value = std::max(
        std::min(
          a_value + (b_value - c_value) * d_value / e_value % f_value,
          g_value),
        h_value);
      RC_ASSERT(expr_value == expected_value);
      // Make sure streaming works correctly
      std::ostringstream ss;
      ss << expr;
      std::string expr_str = ss.str();
      ss.str("");
      ss << a<<" + ("<<b<<" - "<<c<<") * "<<d<<" / "<<e<<" % "<<f<<" min "<<g<<" max "<<h;
      std::string expected_str = ss.str();
      RC_ASSERT(expr_str == expected_str);
    }
  )
);
