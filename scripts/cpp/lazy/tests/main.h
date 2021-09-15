#include <string>
#include <rapidcheck.h>

struct TestBase {
  TestBase();
  virtual ~TestBase();
  virtual void execute() const = 0;
};


template <typename Testcase>
struct Test : public TestBase {
  Test(
    std::string const &inName,
    Testcase const &inTestcase
  ):
    TestBase(),
    mName(inName),
    mTestcase(inTestcase)
  {}

  Test(Test const &inTest): Test(inTest.mName, inTest.mTestcase) {}
  Test(Test &&inTest): Test(inTest.mName, inTest.mTestcase) {}

  void execute() const final {
    rc::check(mName, mTestcase);
  }

  std::string mName;
  Testcase mTestcase;
};


template <typename ...TestTs>
struct Tests {
  std::tuple<TestTs...> mTests;
};


template <typename Testcase>
Test<Testcase> make_test(std::string const &inName, Testcase inTestcase) {
  return Test<Testcase>{inName, inTestcase};
}


template <typename ...TestTs>
Tests<Test<TestTs>...> make_tests(Test<TestTs> &&...inTests) {
  return Tests<Test<TestTs>...>{std::make_tuple(inTests...)};
}


