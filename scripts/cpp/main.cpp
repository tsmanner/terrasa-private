#include <assert.h>
#include <array>
#include <iomanip>
#include <iostream>
#include <string>

#include "entity.h"
#include "lazy/lazy.h"

#include <cxxabi.h>


template <typename T1, typename T2>
struct MergeTypeSets;


template <typename... T1s, typename... T2s>
struct MergeTypeSets<TypeSet<T1s...>, TypeSet<T2s...>> {
  using type = TypeSet<T1s..., T2s...>;
};


template <typename AbilityT, typename EntityT>
struct AddProficiency {
  using Proficiency = typename MergeTypeSets<TypeSet<AbilityT>, typename EntityT::Proficiency>::type;
  using Expertise = typename EntityT::Expertise;
  static constexpr int level = EntityT::level;
  static constexpr int proficiency = EntityT::proficiency;

  AddProficiency(EntityT const &entity): mAbilities(entity.mAbilities) {}

  std::tuple<Strength, Dexterity, Constitution, Intelligence, Wisdom, Charisma> const &mAbilities;
};


template <typename T> struct BullsStrength  : public AddProficiency<Strength,     T> { using AddProficiency<Strength,     T>::AddProficiency; };
template <typename T> struct CatsGrace      : public AddProficiency<Dexterity,    T> { using AddProficiency<Dexterity,    T>::AddProficiency; };
template <typename T> struct BearsEndurance : public AddProficiency<Constitution, T> { using AddProficiency<Constitution, T>::AddProficiency; };
template <typename T> struct FoxsCunning    : public AddProficiency<Intelligence, T> { using AddProficiency<Intelligence, T>::AddProficiency; };
template <typename T> struct OwlsWisdom     : public AddProficiency<Wisdom,       T> { using AddProficiency<Wisdom,       T>::AddProficiency; };
template <typename T> struct EaglesSplendor : public AddProficiency<Charisma,     T> { using AddProficiency<Charisma,     T>::AddProficiency; };

template <typename T> auto bulls_strength( T const &t) { return BullsStrength <T>(t); }
template <typename T> auto cats_grace(     T const &t) { return CatsGrace     <T>(t); }
template <typename T> auto bears_endurance(T const &t) { return BearsEndurance<T>(t); }
template <typename T> auto foxs_cunning(   T const &t) { return FoxsCunning   <T>(t); }
template <typename T> auto owls_wisdom(    T const &t) { return OwlsWisdom    <T>(t); }
template <typename T> auto eagles_splendor(T const &t) { return EaglesSplendor<T>(t); }


namespace lazy {
template <>
struct precedence<std::function<int()>> {
  static constexpr int value = 0;
};
}

int main() {
  using std::cout;

  Entity<11, TypeSet<Dexterity, Intelligence>> TurminderXuss {
    "Turminder Xuss", 10, 19, 10, 16, 12, 10
  };

  // Entity<> Balaam { "Balaam", 11, 14, 12, 15, 10, 10, 20 };

  cout << TurminderXuss;
  cout << "STR " << Save<STR>::of(TurminderXuss) << ", with Bull's Strength "  << Save<STR>::of(bulls_strength( TurminderXuss)) << "\n";
  cout << "DEX " << Save<DEX>::of(TurminderXuss) << ", with Cat's Grace "      << Save<DEX>::of(cats_grace(     TurminderXuss)) << "\n";
  cout << "CON " << Save<CON>::of(TurminderXuss) << ", with Owl's Wisdom "     << Save<CON>::of(owls_wisdom(    TurminderXuss)) << "\n";
  cout << "INT " << Save<INT>::of(TurminderXuss) << ", with Fox's Cunning "    << Save<INT>::of(foxs_cunning(   TurminderXuss)) << "\n";
  cout << "WIS " << Save<WIS>::of(TurminderXuss) << ", with Owl's Wisdom "     << Save<WIS>::of(owls_wisdom(    TurminderXuss)) << "\n";
  cout << "CHA " << Save<CHA>::of(TurminderXuss) << ", with Eagle's Splendor " << Save<CHA>::of(eagles_splendor(TurminderXuss)) << "\n";

  cout << '\n';
  cout << (1 + 2 + d20) << '\n';
  cout << (d20 + 1 + 2) << '\n';
  cout << (1 + d20 + 2) << '\n';

  cout << '\n';
  std::function<int()> f = [](){ return 15; };
  // cout << f << '\n';
  cout << (d20 + f) << '\n';

  return 0;
}
