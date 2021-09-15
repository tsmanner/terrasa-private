#pragma once

#include <ostream>

struct Ability {
  explicit constexpr Ability(int const &inScore): mScore(inScore) {}
  Ability(): mScore(10) {}

  int mScore;
  int mModifier { mScore / 2 - 5 };
};

std::ostream& operator<<(std::ostream& os, Ability const &inAbility) { return os << +inAbility.mScore << "(" << (inAbility.mModifier >= 0 ? "+" : "") << +inAbility.mModifier << ")"; }


struct Strength     : public Ability { using Ability::Ability; }; using STR = Strength;
struct Dexterity    : public Ability { using Ability::Ability; }; using DEX = Dexterity;
struct Constitution : public Ability { using Ability::Ability; }; using CON = Constitution;
struct Intelligence : public Ability { using Ability::Ability; }; using INT = Intelligence;
struct Wisdom       : public Ability { using Ability::Ability; }; using WIS = Wisdom;
struct Charisma     : public Ability { using Ability::Ability; }; using CHA = Charisma;

std::ostream& operator<<(std::ostream& os, Strength     const &inStrength    ) { return os << "STR " << static_cast<Ability const &>(inStrength    ); }
std::ostream& operator<<(std::ostream& os, Dexterity    const &inDexterity   ) { return os << "DEX " << static_cast<Ability const &>(inDexterity   ); }
std::ostream& operator<<(std::ostream& os, Constitution const &inConstitution) { return os << "CON " << static_cast<Ability const &>(inConstitution); }
std::ostream& operator<<(std::ostream& os, Intelligence const &inIntelligence) { return os << "INT " << static_cast<Ability const &>(inIntelligence); }
std::ostream& operator<<(std::ostream& os, Wisdom       const &inWisdom      ) { return os << "WIS " << static_cast<Ability const &>(inWisdom      ); }
std::ostream& operator<<(std::ostream& os, Charisma     const &inCharisma    ) { return os << "CHA " << static_cast<Ability const &>(inCharisma    ); }
