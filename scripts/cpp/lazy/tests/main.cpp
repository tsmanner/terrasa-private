#include <iostream>
#include <set>

#include <fmt/format.h>

#include "main.h"

static std::set<TestBase*> testcases;


TestBase::TestBase() {
  testcases.insert(this);
}


TestBase::~TestBase() {
  testcases.erase(this);
}


int main() {
  for (auto &testcase: testcases) {
    testcase->execute();
  }

  return 0;
}
