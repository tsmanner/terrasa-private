from collections import namedtuple
from functools import reduce
import math
import operator
from random import randint
import statistics
import sys


def conditional(condition, success, failure):
    return success if condition else failure


def switch(condition, cases):
    return cases[condition]


strings = {
    # Compares
    operator.lt: "({} < {})",
    operator.le: "({} <= {})",
    operator.gt: "({} > {})",
    operator.ge: "({} >= {})",
    operator.eq: "({} == {})",
    operator.ne: "({} != {})",
    # Numeric Operators
    operator.add: "({} + {})",
    operator.sub: "({} - {})",
    operator.mul: "({} * {})",
    operator.matmul: "({} @ {})",
    operator.truediv: "({} / {})",
    operator.floordiv: "({} // {})",
    operator.mod: "({} % {})",
    operator.pow: "({} ** {})",
    operator.lshift: "({} << {})",
    operator.rshift: "({} >> {})",
    operator.and_: "({} & {})",
    operator.xor: "({} ^ {})",
    operator.or_: "({} | {})",
    # Others
    conditional: "({} ? {} : {})",
    randint: "[{}:{}]",
    switch: "({})",
    min: "min({}, {})",
    max: "max({}, {})",
}


class Roll:
    def __init__(self, function, *args, **kwargs):
        if isinstance(function, int):
            assert len(args) == 0
            assert len(kwargs) == 0
        self.function = function
        self.args = args
        self.kwargs = kwargs

    # Evaluate the roll
    def __call__(self):
        if isinstance(self.function, int):
            return self.function
        return self.function(
            *[Roll._render_arg(arg) for arg in self.args],
            **{k: Roll._render_arg(v) for k, v in self.kwargs.items()}
        )

    # Generic Combinator
    def __combine__(self, op, *args, **kwargs):
        return Roll(op, self, *args, **kwargs)

    # Compares

    def __lt__(self, other):
        return self.__combine__(operator.lt, other)

    def __le__(self, other):
        return self.__combine__(operator.le, other)

    def __gt__(self, other):
        return self.__combine__(operator.gt, other)

    def __ge__(self, other):
        return self.__combine__(operator.ge, other)

    def __eq__(self, other):
        return self.__combine__(operator.eq, other)

    def __ne__(self, other):
        return self.__combine__(operator.ne, other)

    # Numeric Operators

    def __add__(self, other):
        return self.__combine__(operator.add, other)
    def __radd__(self, other):
        return self.__add__(other)

    def __sub__(self, other):
        return self.__combine__(operator.sub, other)
    def __rsub__(self, other):
        return self.__sub__(other)

    def __mul__(self, other):
        return self.__combine__(operator.mul, other)
    def __rmul__(self, other):
        return self.__mul__(other)

    def __matmul__(self, other):
        return self.__combine__(operator.matmul, other)
    def __rmatmul__(self, other):
        return self.__matmul__(other)

    def __truediv__(self, other):
        return self.__combine__(operator.truediv, other)
    def __rtruediv__(self, other):
        return self.__truediv__(other)

    def __floordiv__(self, other):
        return self.__combine__(operator.floordiv, other)
    def __rfloordiv__(self, other):
        return self.__floordiv__(other)

    def __mod__(self, other):
        return self.__combine__(operator.mod, other)
    def __rmod__(self, other):
        return self.__mod__(other)

    def __pow__(self, other, modulo=None):
        return self.__combine__(operator.pow, other, modulo)
    def __rpow__(self, other, modulo=None):
        return self.__pow__(other, modulo)

    def __lshift__(self, other):
        return self.__combine__(operator.lshift, other)
    def __rlshift__(self, other):
        return self.__lshift__(other)

    def __rshift__(self, other):
        return self.__combine__(operator.rshift, other)
    def __rrshift__(self, other):
        return self.__rshift__(other)

    def __and__(self, other):
        return self.__combine__(operator.and_, other)
    def __rand__(self, other):
        return self.__and__(other)

    def __xor__(self, other):
        return self.__combine__(operator.xor, other)
    def __rxor__(self, other):
        return self.__xor__(other)

    def __or__(self, other):
        return self.__combine__(operator.or_, other)
    def __ror__(self, other):
        return self.__or__(other)

    # Representations
    def __repr__(self):
        if isinstance(self.function, int):
            return repr(self.function)
        return strings[self.function].format(*self.args)

    # For a given argument, if it is a Roll, get a value from it, otherwise pass it through
    @staticmethod
    def _render_arg(arg):
        if isinstance(arg, Roll):
            return arg()
        elif isinstance(arg, int):
            return arg
        elif isinstance(arg, (list, tuple)):
            return [Roll._render_arg(item) for item in arg]
        elif isinstance(arg, dict):
            return {k: Roll._render_arg(v) for k, v in arg.items()}
        raise NotImplementedError(f"{Roll} arguments must be {int} or {Roll} not {type(arg)}, got '{arg}'")


d4  = Roll(randint, 1,  4)
d6  = Roll(randint, 1,  6)
d8  = Roll(randint, 1,  8)
d10 = Roll(randint, 1, 10)
d12 = Roll(randint, 1, 12)
d20 = Roll(randint, 1, 20)


def distance(a, b):
    return round(math.hypot(b.position[0] - a.position[0], b.position[1] - a.position[1]))


disadvantage = lambda attack_roll: Roll(min, attack_roll, attack_roll)
advantage = lambda attack_roll: Roll(max, attack_roll, attack_roll)


def hit(attacker, target):
    return Roll(conditional,
        distance(attacker, target) <= attacker.long_range,
        Roll(conditional,
            distance(attacker, target) <= attacker.short_range,
            attacker.attack_roll,
            disadvantage(attacker.attack_roll),
        ),
        1 + attacker.attack_bonus  # Past long range counts as a natural 1
    )

def attack(attacker, target):
    attack_rolls = {
        1 + attacker.attack_bonus: 0,
        20 + attacker.attack_bonus: attacker.damage + attacker.damage
    }
    attack_rolls.update({
        roll + attacker.attack_bonus: Roll(conditional, attacker.attack_roll >= target.ac, attacker.damage, 0)
        for roll in range(2, 20)
    })
    attacks = [Roll(switch,
        hit(attacker, target),
        attack_rolls
    ) for _ in range(attacker.attacks)]
    return sum(attacks)


def damages(rounds, attacker, target):
    return [attack(attacker, target)() for _ in range(rounds)]


def damage_per_round(rounds, attacker, target):
    return statistics.mean(damages(rounds, attacker, target))


def rounds_to_kill(damage, hp):
    if damage:
        return int(hp // damage + (1 if hp % damage != 0 else 0))
    return "--"


class Entity:
    def __init__(self, name, *, position=None, ac=None, hp=None, attacks=1, attack_bonus=None, short_range=None, long_range=None, damage=None):
        self.name = name
        self.position = position
        self.ac = ac
        self.hp = hp
        self.attacks = attacks
        self.attack_bonus = attack_bonus
        self.attack_roll = d20 + attack_bonus
        self.short_range = short_range
        self.long_range = long_range
        self.damage = damage

    def __str__(self):
        return f"{self.name} {self.position} {self.attack_bonus:+>2} {self.short_range}/{self.long_range}"

    def __repr__(self):
        return str(self)


contestants = [
  Entity("Gluteus Maximus",   position=(0, 0), ac=15, hp=25, attack_bonus=7, short_range=150, long_range=600, damage=2*d8 + d6 + 3),
  Entity("Caltrop Bloodless", position=(0, 0), ac=13, hp=18, attack_bonus=4, short_range= 80, long_range=320, damage=d8 + 2*d6 + 2),
  Entity("Brother Gromag",    position=(0, 0), ac=15, hp=28, attack_bonus=5, short_range= 20, long_range= 60, damage=d4 + 3),
  Entity("Shortbow Guy",      position=(0, 0), ac=13, hp=15, attack_bonus=4, short_range= 80, long_range=320, damage=3*d6 + 3),  # Rogue
  Entity("Longbow Guy",       position=(0, 0), ac=13, hp=15, attack_bonus=6, short_range=150, long_range=600, damage=d8 + d6 + 3),  # Hunter Ranger
  Entity("Javelin Guy",       position=(0, 0), ac=13, hp=15, attack_bonus=7, short_range= 30, long_range=120, damage=d6 + 3, attacks=2),  # Fighter
  Entity("Crossbow Guy",      position=(0, 0), ac=13, hp=15, attack_bonus=4, short_range= 80, long_range=320, damage=d8 + d4 + 3),  # Way of the Kensei Monk
]

targets = {
    Entity("target1", position=(0,  20), ac=13, hp=25):  1,
    Entity("target2", position=(0,  50), ac=14, hp=20):  3,
    Entity("target3", position=(0, 100), ac=15, hp=15):  8,
    Entity("target4", position=(0, 250), ac=16, hp=10): 13,
    Entity("target5", position=(0, 500), ac=17, hp= 5): 20,
}

simulated_rounds = 10000
average_damage_per_round = {
    contestant: {target: damage_per_round(simulated_rounds, contestant, target) for target in targets}
    for contestant in contestants
}

row_format = "| {:>5} | {:>2} | {:>2} ||" + (" {:>2} |" * len(contestants))
print(row_format.format("range", "AC", "HP", "GM", "CB", "BG", "SG", "LG", "JG", "CB"))
for target in targets:
    print(row_format.format(target.position[1], target.ac, target.hp, *[rounds_to_kill(average_damage_per_round[contestant][target], target.hp) for contestant in contestants]))
