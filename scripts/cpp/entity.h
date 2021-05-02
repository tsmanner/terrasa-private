#pragma once

#include <numeric>
#include <set>
#include <functional>

#include "type_classifiers.h"
#include "ability.h"
#include "dice.h"


using typing::Type;


template <typename F, typename T1, typename T2>
constexpr auto fold(F const &inF, T1 const &inT1, T2 const &inT2) {
  return inF(inT1, inT2);
};

template <typename F, typename T1, typename T2, typename... Ts>
constexpr auto fold(F const &inF, T1 const &inT1, T2 const &inT2, Ts const &...inTs) {
  return inF(inT1, fold(inF, inT2, inTs...));
};

template <typename... Ts>
struct TypeSet {
  static constexpr auto types = fold(std::logical_or<>(), Type<Ts>()...);
};

template <typename T>
struct TypeSet<T> {
  static constexpr auto types = Type<T>();
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


template <>
struct TypeSet<> {
  static constexpr auto types = Type<typing::None>();
};


//
// Skills
//

struct Acrobatics {};
struct AnimalHandling {};
struct Arcana {};
struct Athletics {};
struct Concentration {};
struct Deception {};
struct History {};
struct Insight {};
struct Intimidation {};
struct Initiative {};
struct Investigation {};
struct Medicine {};
struct Nature {};
struct Perception {};
struct Performance {};
struct Persuasion {};
struct Religion {};
struct SleightOfHand {};
struct Stealth {};
struct Survival {};

constexpr auto StrengthSkills     = TypeSet<Athletics>::types;
constexpr auto DexteritySkills    = TypeSet<Acrobatics, Initiative, SleightOfHand, Stealth>::types;
constexpr auto ConstitutionSkills = TypeSet<Concentration>::types;
constexpr auto IntelligenceSkills = TypeSet<Arcana, History, Investigation, Nature, Religion>::types;
constexpr auto WisdomSkills       = TypeSet<AnimalHandling, Insight, Medicine, Perception, Survival>::types;
constexpr auto CharismaSkills     = TypeSet<Deception, Intimidation, Performance, Persuasion>::types;


//
// Entity
//

struct Callable {
  std::function<int()> f;

};


template <typename AbilityT>
struct Check {
  template <typename EntityT>
  static constexpr auto of(EntityT const &entity) { return d20 + std::get<AbilityT>(entity.mAbilities).mModifier; }

};

template <typename AbilityT>
struct Save {
  template <typename EntityT, typename std::enable_if_t<(Type<AbilityT>() == (!EntityT::Proficiency::types and !EntityT::Expertise::types)), bool>...>
  static constexpr auto of(EntityT const &entity) { return Check<AbilityT>::of(entity); }

  template <typename EntityT, typename std::enable_if_t<(Type<AbilityT>() == (EntityT::Proficiency::types and !EntityT::Expertise::types)), bool>...>
  static constexpr auto of(EntityT const &entity) { return Check<AbilityT>::of(entity) + entity.proficiency; }

  template <typename EntityT, typename std::enable_if_t<(Type<AbilityT>() == EntityT::Expertise::types), bool>...>
  static constexpr auto of(EntityT const &entity) { return Check<AbilityT>::of(entity) + entity.proficiency + entity.proficiency; }

};


template <int Level, typename ProficiencyT = TypeSet<>, typename ExpertiseT = TypeSet<>>
struct Entity {
  using Proficiency = ProficiencyT;
  using Expertise = ExpertiseT;
  static constexpr int level = Level;
  static constexpr int proficiency = Level / 5 + 2;

  Entity(
    Name         const &inName,
    Strength     const &inStrength,
    Dexterity    const &inDexterity,
    Constitution const &inConstitution,
    Intelligence const &inIntelligence,
    Wisdom       const &inWisdom,
    Charisma     const &inCharisma
  ):
    mName(inName),
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
    set(inTs...);
  }

  // TODO:
  //   Implement a set function for each type, then
  //   a variadic that recurses to call each one.  That
  //   should then allow a variadic constructor that
  //   approximates using keyword arguments.
  void set(Name         const &inName        ) {                        mName       = inName;         }
  // void set(Level        const &inLevel       ) {                        mLevel      = inLevel;        }
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


  template <typename T, typename std::enable_if_t<(typing::Type<T>() != Proficiency()), bool>...>
  inline auto check() const { return d20 + std::get<T>(mAbilities).mModifier; }

  // inline auto proficiency(Ability const &ability) const { return check(ability) + proficiency(); }
  // inline auto expertise(Ability const &ability) const { return proficiency(ability) + proficiency(); }


  Name mName;
  std::tuple<Strength, Dexterity, Constitution, Intelligence, Wisdom, Charisma> mAbilities;

};


template <int Level, typename... Ts>
std::ostream &operator<<(std::ostream &os, Entity<Level, Ts...> const &inEntity) {
  return os << inEntity.mName << ", Level " << +inEntity.level
    << "\n  " << std::get<Strength>(inEntity.mAbilities)
    << "\n  " << std::get<Dexterity>(inEntity.mAbilities)
    << "\n  " << std::get<Constitution>(inEntity.mAbilities)
    << "\n  " << std::get<Intelligence>(inEntity.mAbilities)
    << "\n  " << std::get<Wisdom>(inEntity.mAbilities)
    << "\n  " << std::get<Charisma>(inEntity.mAbilities)
    << "\n";
}

