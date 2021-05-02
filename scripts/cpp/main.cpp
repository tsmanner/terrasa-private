#include <assert.h>
#include <array>
#include <iomanip>
#include <iostream>
#include "entity.h"
#include "roll.h"


int main() {
  using std::cout;

  // TODO API Goal: kwargs approximation
  Entity<Dexterity, Constitution> TurminderXuss {
    Name("Turminder Xuss"),
    Level(11),
    Dexterity(19),
    Intelligence(16),
    Wisdom(12),
    Strength(10),
    Constitution(10),
    Charisma(10)
  };

  // Entity<> Balaam { "Balaam", 11, 14, 12, 15, 10, 10, 20 };
  cout << TurminderXuss;
  cout << std::is_same<decltype(typing::Type<Dexterity>() || typing::Type<Constitution>()), decltype(decltype(TurminderXuss)::Proficiencies)>::value << '\n';
  // cout << TurminderXuss.check<Dexterity>() << "\n";

  // cout << fold(std::plus<>(), 1, 2, 3) << '\n';

  // cout << TurminderXuss

  return 0;
}
