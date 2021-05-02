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


template <typename T>
struct Value {
  explicit Value(T const &inValue): mValue(inValue) {}
  Value() {}
  T mValue;
};


struct Level : public Value<int> {
  using Value<int>::Value;
  int proficiency() const { return mValue / 5 + 2; }
};
std::ostream &operator<<(std::ostream &os, Level const &inLevel) { return os << "Level " << inLevel.mValue; }

struct Name : public Value<std::string> { using Value<std::string>::Value; };
std::ostream &operator<<(std::ostream &os, Name const &inName) { return os << inName.mValue; }


template <typename... ProficientTypes>
struct Entity {

  static constexpr auto Proficiencies = fold(std::logical_or<>(), typing::Type<ProficientTypes>()...);

  Entity(
    Name         const &inName,
    Level        const &inLevel,
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

  template <typename... Ts>
  Entity(Ts const &... inTs) {
    set(inTs)...;
  }

  // TODO:
  //   Implement a set function for each type, then
  //   a variadic that recurses to call each one.  That
  //   should then allow a variadic constructor that
  //   approximates using keyword arguments.
  void set(Name         const &inName        ) {                        mName       = inName;         }
  void set(Level        const &inLevel       ) {                        mLevel      = inLevel;        }
  void set(Strength     const &inStrength    ) { std::get<Strength    >(mAbilities) = inStrength;     }
  void set(Dexterity    const &inDexterity   ) { std::get<Dexterity   >(mAbilities) = inDexterity;    }
  void set(Constitution const &inConstitution) { std::get<Constitution>(mAbilities) = inConstitution; }
  void set(Intelligence const &inIntelligence) { std::get<Intelligence>(mAbilities) = inIntelligence; }
  void set(Wisdom       const &inWisdom      ) { std::get<Wisdom      >(mAbilities) = inWisdom;       }
  void set(Charisma     const &inCharisma    ) { std::get<Charisma    >(mAbilities) = inCharisma;     }

  template <typename T, typename... Ts>
  void set(T const &inT, Ts const &... inTs) {
    set(inT);
    set(inTs...);
  }


  int proficiency() const {
    return mLevel.proficiency();
  }

  template <typename T, typename std::enable_if_t<(typing::Type<T>() != Proficiencies), bool>...>
  inline auto check() const { return d20 + std::get<T>(mAbilities).mModifier; }

  // inline auto proficiency(Ability const &ability) const { return check(ability) + proficiency(); }
  // inline auto expertise(Ability const &ability) const { return proficiency(ability) + proficiency(); }


  Name mName;
  Level mLevel;
  std::tuple<Strength, Dexterity, Constitution, Intelligence, Wisdom, Charisma> mAbilities;

};


template <typename... Ts>
std::ostream &operator<<(std::ostream &os, Entity<Ts...> const &inEntity) {
  return os << inEntity.mName << ", " << inEntity.mLevel
    << "\n  " << std::get<Strength>(inEntity.mAbilities)
    << "\n  " << std::get<Dexterity>(inEntity.mAbilities)
    << "\n  " << std::get<Constitution>(inEntity.mAbilities)
    << "\n  " << std::get<Intelligence>(inEntity.mAbilities)
    << "\n  " << std::get<Wisdom>(inEntity.mAbilities)
    << "\n  " << std::get<Charisma>(inEntity.mAbilities)
    << "\n";
}

