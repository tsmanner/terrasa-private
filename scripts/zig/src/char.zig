//! Backend Data Model
//!     Character
//!         Proficiency
//!         Abilities (score, bonuses)
//!             Strength
//!             Dexterity
//!             Constitution
//!             Intelligence
//!             Wisdom
//!             Charisma
//!     Modifiers
//!         Spells and Effects
//!         Equipment
//!             Armor
//!             Weapons
//! Frontend
//!     Character
//!         Proficiency ?
//!         Ability Checks (adv, dadv)
//!             Strength
//!             Dexterity
//!             Constitution
//!             Intelligence
//!             Wisdom
//!             Charisma
//!         Ability Saves (adv, dadv)
//!             Strength
//!             Dexterity
//!             Constitution
//!             Intelligence
//!             Wisdom
//!             Charisma
//!         Skills (modifier)
//!             Athletics
//!             ...
//!         Equipment
//!             Armor
//!             Weapon(s)
//!             Footwear
//!             Jewelery
//!             Rod / Wand / Etc
//!             Misc
//!         Armor Class
//!             - 8 + Dexterity Modifier
//!             - Armor
//!             - Class Feature (e.g. Unarmored Defense)
//!             - Spell Effect (e.g. Mage Armor)
//!         Spell Save DC

const std = @import("std");
