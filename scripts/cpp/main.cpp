#include <assert.h>
#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

#include "entity.h"
#include "lazy/lazy.h"

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>

#include <cxxabi.h>
#include <dlfcn.h>


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



void dll() {
  using std::cout;

  void  *dl = dlopen("./TurminderXuss.so", RTLD_LAZY);
  if (dl == nullptr) { throw  std::runtime_error("dlopen failure!"); }

  int (*strSave)();
  strSave = (decltype(strSave))(dlsym(dl, "strengthSave"));
  strSave = (decltype(strSave))(dlsym(dl, "strengthSave()"));

  strSave = (decltype(strSave))(dlsym(dl, "_Z12strengthSavev"));
  if (strSave == nullptr) { throw  std::runtime_error("dlsym 'strengthSave' not found!"); }

  cout << strSave();
  cout << '\n';

  BaseEntity *TurminderXuss = (decltype(TurminderXuss))(dlsym(dl, "_ZL13TurminderXuss"));
  if (TurminderXuss == nullptr) { throw  std::runtime_error("dlsym 'TurminderXuss' not found!"); }

  cout << TurminderXuss->strengthSave();
  cout << '\n';

  dlclose(dl);
}

constexpr auto row_format = "{:3} {:<11} or {:<11} with {}\n";

int main() {
  using std::cout;

  // TODO API Goal: kwargs approximation
  Entity<11, TypeSet<Dexterity, Constitution>> TurminderXuss {
    Name("Turminder Xuss"),
    Dexterity(19),
    Intelligence(16),
    Wisdom(12),
    Strength(10),
    Constitution(10),
    Charisma(10)
  };

  // Entity<> Balaam { "Balaam", 11, 14, 12, 15, 10, 10, 20 };

  // cout << TurminderXuss;
  fmt::print(fmt::fg(fmt::color::indian_red   ), row_format, "STR", Save<STR>::of(TurminderXuss), Save<STR>::of(bulls_strength( TurminderXuss)), "Bull's Strength");
  fmt::print(fmt::fg(fmt::color::dark_green   ), row_format, "DEX", Save<DEX>::of(TurminderXuss), Save<STR>::of(cats_grace(     TurminderXuss)), "Cat's Grace");
  fmt::print(fmt::fg(fmt::color::orange       ), row_format, "CON", Save<CON>::of(TurminderXuss), Save<STR>::of(owls_wisdom(    TurminderXuss)), "Owl's Wisdom");
  fmt::print(fmt::fg(fmt::color::royal_blue   ), row_format, "INT", Save<INT>::of(TurminderXuss), Save<STR>::of(foxs_cunning(   TurminderXuss)), "Fox's Cunning");
  fmt::print(fmt::fg(fmt::color::green_yellow ), row_format, "WIS", Save<WIS>::of(TurminderXuss), Save<STR>::of(owls_wisdom(    TurminderXuss)), "Owl's Wisdom");
  fmt::print(fmt::fg(fmt::color::medium_purple), row_format, "CHA", Save<CHA>::of(TurminderXuss), Save<STR>::of(eagles_splendor(TurminderXuss)), "Eagle's Splendor");

  // cout << '\n';
  // cout << (1 + 2 + d20) << '\n';
  // cout << (d20 + 1 + 2) << '\n';
  // cout << (1 + d20 + 2) << '\n';

  // cout << '\n';
  // std::function<int()> f = [](){ return 15; };
  // cout << (d20 + f) << " = " << (d20 + f)() << '\n';

  return 0;
}
