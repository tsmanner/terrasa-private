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
    std::string  const &inName,
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

  std::string mName;
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

