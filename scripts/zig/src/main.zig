const std = @import("std");


pub fn Int(comptime max_value: u32) type {
    return @Type(std.builtin.TypeInfo{
        .Int = .{
            .signedness = .unsigned,
            .bits = std.math.log2_int_ceil(@TypeOf(max_value), max_value),
        }
    });
}

var prng = std.rand.DefaultPrng.init(blk: {
    var seed: u64 = undefined;
    try std.os.getrandom(std.mem.asBytes(&seed));
    break :blk seed;
});

pub fn Random(min: anytype, max: anytype) type {
    return struct {
        const Self = @This();
        const min = min;
        const max = max;
        pub fn draw(self: Self) Int(max) {
            return prng.random.intRangeAtMost(Int(max), min, max);
        }
    };
}

pub fn random(min: anytype, max: anytype) Random(min, max) {
    return .{};
}

var d20 = random(1, 20);


pub fn main() anyerror!void {
    std.log.info("1d20 -> {}\n", .{d20.draw()});
}
