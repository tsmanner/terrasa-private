#pragma once

#include "roll.h"


struct Ability {
  explicit constexpr Ability(int const &inScore): mScore(inScore) {}
  Ability(): mScore(10) {}

  int const mScore;
  int const mModifier { mScore / 2 - 5 };
};

std::ostream& operator<<(std::ostream& os, Ability const &inAbility) { return os << +inAbility.mScore << "(" << +inAbility.mModifier << ")"; }


template <typename T>
struct Proficiency {

};




struct Strength     : public Ability { using Ability::Ability; };
struct Dexterity    : public Ability { using Ability::Ability; };
struct Constitution : public Ability { using Ability::Ability; };
struct Intelligence : public Ability { using Ability::Ability; };
struct Wisdom       : public Ability { using Ability::Ability; };
struct Charisma     : public Ability { using Ability::Ability; };

std::ostream& operator<<(std::ostream& os, Strength     const &inStrength    ) { return os << "STR " << static_cast<Ability const &>(inStrength    ); }
std::ostream& operator<<(std::ostream& os, Dexterity    const &inDexterity   ) { return os << "DEX " << static_cast<Ability const &>(inDexterity   ); }
std::ostream& operator<<(std::ostream& os, Constitution const &inConstitution) { return os << "CON " << static_cast<Ability const &>(inConstitution); }
std::ostream& operator<<(std::ostream& os, Intelligence const &inIntelligence) { return os << "INT " << static_cast<Ability const &>(inIntelligence); }
std::ostream& operator<<(std::ostream& os, Wisdom       const &inWisdom      ) { return os << "WIS " << static_cast<Ability const &>(inWisdom      ); }
std::ostream& operator<<(std::ostream& os, Charisma     const &inCharisma    ) { return os << "CHA " << static_cast<Ability const &>(inCharisma    ); }

constexpr auto operator "" _STR(unsigned long long int score) { return Strength(score    ); }
constexpr auto operator "" _DEX(unsigned long long int score) { return Dexterity(score   ); }
constexpr auto operator "" _CON(unsigned long long int score) { return Constitution(score); }
constexpr auto operator "" _INT(unsigned long long int score) { return Intelligence(score); }
constexpr auto operator "" _WIS(unsigned long long int score) { return Wisdom(score      ); }
constexpr auto operator "" _CHA(unsigned long long int score) { return Charisma(score    ); }
