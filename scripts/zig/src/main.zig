const std = @import("std");

pub fn Int(comptime max_value: u32) type {
    return @Type(std.builtin.TypeInfo{ .Int = .{
        .signedness = .unsigned,
        .bits = std.math.log2_int_ceil(@TypeOf(max_value), max_value),
    } });
}

pub fn Random(min: anytype, max: anytype) type {
    return struct {
        const Self = @This();
        const min = min;
        const max = max;

        prng: *std.rand.DefaultPrng = prng,

        pub fn draw(self: Self) Int(max) {
            return self.prng.random.intRangeAtMost(Int(max), min, max);
        }
    };
}

pub fn random(min: anytype, max: anytype, prng: anytype) Random(min, max) {
    return .{ .prng = prng };
}

const DnD = struct {
    d4: Random(1, 4),
    d6: Random(1, 6),
    d8: Random(1, 8),
    d10: Random(1, 10),
    d12: Random(1, 12),
    d20: Random(1, 20),

    pub fn roll(Dnd: self, comptime s: []u8) u8 {}
};

pub fn get_dnd(prng: anytype) anyerror!DnD {
    return DnD{
        .d4 = Random(1, 4){ .prng = prng },
        .d6 = Random(1, 6){ .prng = prng },
        .d8 = Random(1, 8){ .prng = prng },
        .d10 = Random(1, 10){ .prng = prng },
        .d12 = Random(1, 12){ .prng = prng },
        .d20 = Random(1, 20){ .prng = prng },
    };
}

pub fn main() anyerror!void {
    var prng = std.rand.DefaultPrng.init(blk: {
        var seed: u64 = undefined;
        try std.os.getrandom(std.mem.asBytes(&seed));
        break :blk seed;
    });
    var dnd = try get_dnd(&prng);
    std.log.info("1d20 -> {}\n", .{dnd.d20.draw()});
}
