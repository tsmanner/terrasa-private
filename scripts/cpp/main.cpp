#include <assert.h>
#include <array>
#include <iomanip>
#include <iostream>
#include "entity.h"
#include "roll.h"


int main() {
  using std::cout;

  Entity<Dexterity, Constitution> TurminderXuss { "Turminder Xuss", 11, 10, 19, 10, 16, 12, 10 };
  // Entity<> Balaam { "Balaam", 11, 14, 12, 15, 10, 10, 20 };
  cout << TurminderXuss;
  cout << std::is_same<decltype(typing::Type<Dexterity>() || typing::Type<Constitution>()), decltype(decltype(TurminderXuss)::Proficiencies)>::value << '\n';
  // cout << TurminderXuss.check<Dexterity>() << "\n";

  // cout << fold(std::plus<>(), 1, 2, 3) << '\n';

  // cout << TurminderXuss

  return 0;
}
