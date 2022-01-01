const std = @import("std");

// pub fn Int(comptime max_value: u32) type {
//     return @Type(std.builtin.TypeInfo{ .Int = .{
//         .signedness = .unsigned,
//         .bits = std.math.log2_int_ceil(@TypeOf(max_value), max_value),
//     } });
// }

// const Random = struct {
//     const Self = @This();

//     min: Int(max),
//     max: Int(max),

//     pub fn draw(self: Self, prng: *std.rand.DefaultPrng) Int(max) {
//         return prng.random.intRangeAtMost(Int(self.max), self.min, self.max);
//     }
// };

// pub fn random(min: anytype, max: anytype, prng: anytype) Random {
//     return .{ .min = min, .max = max, .prng = prng };
// }

// const DnD = struct {
//     d4: Random,
//     d6: Random,
//     d8: Random,
//     d10: Random,
//     d12: Random,
//     d20: Random,

//     pub fn roll(Dnd: self, comptime s: []u8) u8 {
//         inline for (s) |c| {}
//     }
// };

// pub fn get_dnd(prng: anytype) DnD {
//     return DnD{
//         .d4 = random(1, 4, prng),
//         .d6 = random(1, 6, prng),
//         .d8 = random(1, 8, prng),
//         .d10 = random(1, 10, prng),
//         .d12 = random(1, 12, prng),
//         .d20 = random(1, 20, prng),
//     };
// }

// TODO: Write a comptime parser for dice arithmetic.
// - [ ]  N   modifiers
// - [ ] XdY dice
// - [ ]  +  addition
// - [ ]  -  subtraction
// - [ ]  *  multiplication
// - [ ] ??? advantage
// - [ ] ??? disadvantage
// - [ ]  == equal
// - [ ]  <  less
// - [ ]  <= less equal
// - [ ]  >  greater
// - [ ]  >= greater equal

pub fn Roll(comptime s: []const u8) type {
    return struct {
        const Self = @This();
        const equation: []const u8 = s;
        pub fn roll() u32 {
            return 0;
        }
    };
}

fn parse_modifier(comptime s: []const u8) comptime_int {
    inline for (s) |c| {
        if (c == ' ' || c == '\t') {
            continue;
        } else if (c >= '0' && c <= '9') {
            switch(c)
        }
    }
}

test "Parse Modifier" {

}

pub fn main() anyerror!void {
    // var prng = std.rand.DefaultPrng.init(blk: {
    //     var seed: u64 = undefined;
    //     try std.os.getrandom(std.mem.asBytes(&seed));
    //     break :blk seed;
    // });
    // var dnd = get_dnd(&prng);
    const roll = Roll("1d20 + 8");
    std.log.info("{s} -> {}\n", .{ roll.equation, roll.roll() });
}
