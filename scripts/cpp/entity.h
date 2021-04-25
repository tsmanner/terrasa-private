#pragma once

#include <numeric>
#include <set>

#include "type_classifiers.h"
#include "ability.h"


template <typename F, typename T1, typename T2>
constexpr auto fold(F &&inF, T1 &&inT1, T2 &&inT2) {
  return inF(inT1, inT2);
};

template <typename F, typename T1, typename T2, typename... Ts>
constexpr auto fold(F &&inF, T1 &&inT1, T2 &&inT2, Ts &&...inTs) {
  return inF(inT1, fold(inF, inT2, inTs...));
};



template <typename... ProficientTypes>
struct Entity {

  static constexpr auto Proficiencies = fold(std::logical_or<>(), typing::Type<ProficientTypes>()...);

  Entity(
    std::string const &inName,
    int const &inLevel,
    Strength     const &inStrength,
    Dexterity    const &inDexterity,
    Constitution const &inConstitution,
    Intelligence const &inIntelligence,
    Wisdom       const &inWisdom,
    Charisma     const &inCharisma
  ):
    mName(inName),
    mLevel(inLevel),
    mAbilities(
      inStrength,
      inDexterity,
      inConstitution,
      inIntelligence,
      inWisdom,
      inCharisma
    )
  {}

  int proficiency() const {
    return mLevel / 5 + 2;
  }

  template <typename T, typename std::enable_if_t<(typing::Type<T>() != Proficiencies), bool>...>
  inline auto check() const { return d20 + std::get<T>(mAbilities).mModifier; }

  // inline auto proficiency(Ability const &ability) const { return check(ability) + proficiency(); }
  // inline auto expertise(Ability const &ability) const { return proficiency(ability) + proficiency(); }


  std::string mName;
  int mLevel;
  std::tuple<Strength, Dexterity, Constitution, Intelligence, Wisdom, Charisma> mAbilities;

};


template <typename... Ts>
std::ostream &operator<<(std::ostream &os, Entity<Ts...> const &inEntity) {
  return os << inEntity.mName << ", Level " << +inEntity.mLevel
    << "\n  " << std::get<Strength>(inEntity.mAbilities)
    << "\n  " << std::get<Dexterity>(inEntity.mAbilities)
    << "\n  " << std::get<Constitution>(inEntity.mAbilities)
    << "\n  " << std::get<Intelligence>(inEntity.mAbilities)
    << "\n  " << std::get<Wisdom>(inEntity.mAbilities)
    << "\n  " << std::get<Charisma>(inEntity.mAbilities)
    << "\n";
}

