from collections import namedtuple
from functools import reduce
import random


class Attacker:
    def __init__(self, name, attack_bonus, short_range, long_range):
        self.name = name
        self.attack_bonus = attack_bonus
        self.short_range = short_range
        self.long_range = long_range

    def __str__(self):
        return f"{self.name} {self.attack_bonus:+>3} {self.short_range}/{self.long_range}"

    def __repr__(self):
        return str(self)

    def attack(self, target):
        if target.distance <= self.short_range:
            roll = random.randint(1, 20)
        elif target.distance <= self.long_range:
            roll = min(random.randint(1, 20), random.randint(1, 20))
        else:
            roll = 0
        if roll == 20:
            return target.score * 2
        elif roll >= target.ac:
            return target.score
        return 0


Target = namedtuple("Target", ["ac", "distance", "score"])


class HitChance:
    def __init__(self, attacker: Attacker, target: Target):
        self.attacker = attacker
        self.target = target
        hit = 0.05 * (sum([1 for roll in range(2, 20) if (attacker.attack_bonus + roll) >= target.ac]))
        if target.distance <= attacker.short_range:
            self.hit_chance = hit
            self.crit_chance = 0.05
        elif target.distance <= attacker.long_range:
            self.hit_chance = hit ** 2
            self.crit_chance = 0.05 ** 2
        else:
            self.hit_chance = 0
            self.crit_chance = 0

    @property
    def ev(self):
        return self.hit_chance*self.target.score + self.crit_chance*self.target.score*2

    def __str__(self):
        return f"{self.attacker.name:<17} {self.attacker.attack_bonus:+>2} -> {self.target.distance:>3} : hit={self.hit_chance*100:>5.2f}% crit={self.crit_chance*100:>4.2f}% ev={self.ev:>4.2f}"


attackers = [
    Attacker("Brother Gromag",    5,  20,  60),
    Attacker("Caltrop Bloodless", 4,  80, 320),
    Attacker("Gluteus Maximus",   7, 150, 600),
    Attacker("Shortbow Guy",      4,  80, 320),
    Attacker("Longbow Guy",       4, 150, 600),
    Attacker("Javelin Guy",       4,  30, 120),
    Attacker("Crossbow Guy",      4,  80, 320),
]


target_ac = 15
targets = [
    Target(target_ac,  20,  1),
    Target(target_ac,  50,  3),
    Target(target_ac, 100,  4),
    Target(target_ac, 150,  8),
    Target(target_ac, 300, 13),
    Target(target_ac, 500, 20),
]


preferred_targets = {
    attacker: reduce(lambda best, current: best if HitChance(attacker, best).ev > HitChance(attacker, current).ev else current, targets, targets[0]) 
    for attacker in attackers
}
preferred_targets[attackers[2]] = targets[3]
preferred_targets[attackers[4]] = targets[3]

class Tournament:
    def __init__(self):
        self.scores = self._execute()
        max_score = max([sum(v) for k, v in self.scores.items()])
        winner = [competitor for competitor, score in self.scores.items() if sum(score) == max_score]
        if len(winner) == 1:
            self.winner = winner[0]
        else:
            self.winner = None

    def _execute(self):

        scores = {attacker: list() for attacker in attackers}
        for _ in range(10):
            [scores[attacker].append(attacker.attack(preferred_targets[attacker])) for attacker in attackers]
        return scores


if __name__ == "__main__":
    [print(HitChance(attackers[2], target)) for target in targets]
    [print(HitChance(k, v)) for k, v in preferred_targets.items()]
    # [print(f"{k.name:<17} : {v.distance:>3} ev={HitChance(k, v).ev:>4.2f}") for k, v in preferred_targets.items()]
    tournaments = [Tournament() for _ in range(10000)]
    wins = {attacker: 0 for attacker in attackers}
    for tournament in tournaments:
        if tournament.winner:
            wins[tournament.winner] += 1
    misses = {attacker: sum([tournament.scores[attacker].count(0) for tournament in tournaments]) for attacker in attackers}
    total = {attacker: sum([len(tournament.scores[attacker]) for tournament in tournaments]) for attacker in attackers}
    [print(f"{k.name:<17} : {v:>4} | {misses[k]:>5}/{total[k]}") for k, v in reversed(sorted(wins.items(), key=lambda kv: kv[1]))]
