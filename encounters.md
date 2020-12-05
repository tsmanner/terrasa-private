<img src='https://g.gravizo.com/svg?
  digraph G {
    label = "Slums"
    labelloc = "t"
    fontsize = 24
    pad = 0.25
    "Infestation: Theft" -> "Heist: Attacking" -> "Caught!"
  }
'>

<img src='https://g.gravizo.com/svg?
  digraph G {
    label = "Town Hall"
    labelloc = "t"
    fontsize = 24
    pad = 0.25
    "Infestation: Investigate" -> "Heist - attacking" -> "Caught!"
  }
'>

<img src='https://g.gravizo.com/svg?
  digraph G {
    label = "Town Hall"
    labelloc = "t"
    fontsize = 24
    pad = 0.25
    "Infestation: Investigate" -> ""
  }
'>

## Encounter Progressions
---

Slums:
  - `Infestation` to "recover" some grain to sell on the black market (1gp each)
  - `Heist - attacking` You've proven yourselves, search a caravan for their writ of passage and deliver it to us.  Anything else you find, you are welcome to keep.
  - `Caught!`
  - If escape and return to Heist mission giver - see signs of undead, possibly end up meeting "Kingping" Sayre for next mission.  Bodyguards are disguised undead - DC 15 perception or physical contact + DC 8.

Town Hall / Militia Barracks:
  - `Infestation`, notice signs of theft
  - Investigation leads to `Kingpin`
    - Visit up to 4 locations, each giving one piece of information:
      1. Location
      2. Trigger `Bandit Chase` - Success gives information about the opportune time and way to enter (no thugs to fight on the way in)
      3. Rumors of traps in the den
      4. Nuffin'
  - Find and attempt to enter the Bandit Den.
    - Two Thugs at the door, on sitting on the ground dressed in shabby clothes to look like a beggar, one just inside.
      - Skippable if party took part in `Bandit Chase` and caught him or saw him escape in the side door, and chooses to use it.  DC 15 Perception to notice the side door themselves.
    - Inside, in the main mess, between door and the Kingpin's dining chamber, there's another pair of Thugs.
      - Skip if party succeeded in `Bandit Chase` and learned when to enter to avoid the most people.
    - `Kingpin`

Market:
  - `Heist - guarding`
  - Investigation leads to `Kingpin`
      - Visit up to 4 locations, each giving one piece of information:
        1. Location
        2. Trigger `Bandit Chase` - Success gives information about the opportune time and way to enter (no thugs to fight on the way in)
        3. Rumors of traps in the den
        4. Nuffin'
  - Find and attempt to enter the Bandit Den.
    - Two Thugs at the door, on sitting in a chair dressed in shabby clothes to look like a beggar, one just inside.
      - Skip if party took part in `Bandit Chase` and caught him or saw him escape in the side door.
    - Inside, in the main mess, between door and the Kingpin's dining chamber, there's another pair of Thugs.
      - Skip if party succeeded in `Bandit Chase` and learned when to enter to avoid the most people.
    - `Kingpin`


