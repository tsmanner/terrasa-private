#include "entity.h"
#include "lazy/lazy.h"

static Entity<11, TypeSet<Dexterity, Constitution>> TurminderXuss_t {
  Name("Turminder Xuss"),
  Dexterity(19),
  Intelligence(16),
  Wisdom(12),
  Strength(10),
  Constitution(10),
  Charisma(10)
};

static BaseEntity *TurminderXuss = &TurminderXuss_t;

// extern "C" {

int strengthSave() {
  return Save<Strength>::of(TurminderXuss_t)();
}

// }
