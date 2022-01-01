//! Die representation, roll parser, and json serializer

const std = @import("std");

pub const Die = struct {
    d: i32,
};

pub const d4 = Die{ .d = 4 };
pub const d6 = Die{ .d = 6 };
pub const d8 = Die{ .d = 8 };
pub const d10 = Die{ .d = 10 };
pub const d12 = Die{ .d = 12 };
pub const d20 = Die{ .d = 20 };

const Unary = struct {
    const Operation = enum {
        // Arithmetic
        negate,
        // Logical
        logical_not,

        pub fn jsonStringify(self: Operation, _: std.json.StringifyOptions, out_stream: anytype) @TypeOf(out_stream).Error!void {
            try out_stream.writeByte('"');
            try out_stream.writeAll(@tagName(self));
            try out_stream.writeByte('"');
        }
    };

    op: Operation,
    operand: *Value,

    pub fn get(self: Unary, random: std.rand.Random) Value.Result {
        return switch (self.op) {
            .negate => .{ .Integer = -self.operand.get(random).Integer },
            .logical_not => .{ .Bool = !self.operand.get(random).Bool },
        };
    }

    /// For operators, frees all operand pointers
    pub fn deinit(self: *Unary, allocator: std.mem.Allocator) void {
        switch (self.*.operand.*) {
            .Constant, .Bool, .Die => {},
            .Unary => |*operand| operand.*.deinit(allocator),
            .Binary => |*operand| operand.*.deinit(allocator),
        }
        allocator.destroy(self.*.operand);
    }
};

const Binary = struct {
    const Operation = enum {
        // Arithmetic
        add,
        subtract,
        multiply,
        divide,
        minimum,
        maximum,
        // Relational
        equal,
        less,
        less_equal,
        greater,
        greater_equal,
        // Logical
        logical_and,
        logical_or,

        pub fn jsonStringify(self: Operation, _: std.json.StringifyOptions, out_stream: anytype) @TypeOf(out_stream).Error!void {
            try out_stream.writeByte('"');
            try out_stream.writeAll(@tagName(self));
            try out_stream.writeByte('"');
        }
    };

    op: Operation,
    lhs: *Value,
    rhs: *Value,

    pub fn get(self: Binary, random: std.rand.Random) Value.Result {
        return switch (self.op) {
            // Arithmetic
            .add => .{ .Integer = self.lhs.get(random).Integer + self.rhs.get(random).Integer },
            .subtract => .{ .Integer = self.lhs.get(random).Integer - self.rhs.get(random).Integer },
            .multiply => .{ .Integer = self.lhs.get(random).Integer * self.rhs.get(random).Integer },
            .divide => .{ .Integer = divide(self.lhs.get(random).Integer, self.rhs.get(random).Integer) },
            .minimum => .{ .Integer = @minimum(self.lhs.get(random).Integer, self.rhs.get(random).Integer) },
            .maximum => .{ .Integer = @maximum(self.lhs.get(random).Integer, self.rhs.get(random).Integer) },
            // Relational
            .equal => .{ .Bool = self.lhs.get(random).Integer == self.rhs.get(random).Integer },
            .less => .{ .Bool = self.lhs.get(random).Integer < self.rhs.get(random).Integer },
            .less_equal => .{ .Bool = self.lhs.get(random).Integer <= self.rhs.get(random).Integer },
            .greater => .{ .Bool = self.lhs.get(random).Integer > self.rhs.get(random).Integer },
            .greater_equal => .{ .Bool = self.lhs.get(random).Integer >= self.rhs.get(random).Integer },
            // Logical
            .logical_and => .{ .Bool = self.lhs.get(random).Bool and self.rhs.get(random).Bool },
            .logical_or => .{ .Bool = self.lhs.get(random).Bool or self.rhs.get(random).Bool },
        };
    }

    /// D&D division always rounds up
    fn divide(lhs: i32, rhs: i32) i32 {
        if (@rem(lhs, rhs) == 0) {
            return @divFloor(lhs, rhs);
        }
        return @divFloor(lhs, rhs) + 1;
    }

    /// Deinit and free both operands
    pub fn deinit(self: *Binary, allocator: std.mem.Allocator) void {
        switch (self.*.lhs.*) {
            .Constant, .Bool, .Die => {},
            .Unary => |*lhs| lhs.*.deinit(allocator),
            .Binary => |*lhs| lhs.*.deinit(allocator),
        }
        switch (self.*.rhs.*) {
            .Constant, .Bool, .Die => {},
            .Unary => |*rhs| rhs.*.deinit(allocator),
            .Binary => |*rhs| rhs.*.deinit(allocator),
        }
        allocator.destroy(self.*.lhs);
        allocator.destroy(self.*.rhs);
    }
};

test "Binary to json to Binary" {
    var out_buf: [1024]u8 = undefined;
    var slice_stream = std.io.fixedBufferStream(&out_buf);
    const out = slice_stream.writer();
    var lhs = Value{ .Constant = 10 };
    var rhs = Value{ .Constant = 5 };
    const b1 = Binary{
        .op = .add,
        .lhs = &lhs,
        .rhs = &rhs,
    };
    try std.json.stringify(b1, .{}, out);
    try std.testing.expect(std.mem.eql(u8, "{\"op\":\"add\",\"lhs\":10,\"rhs\":5}", slice_stream.getWritten()));

    const options = std.json.ParseOptions{ .allocator = std.testing.allocator };
    const b2 = try std.json.parse(Binary, &std.json.TokenStream.init(slice_stream.getWritten()), options);
    defer std.json.parseFree(Binary, b2, options);
    try std.testing.expectEqual(b1.op, b2.op);
    try std.testing.expectEqual(b1.lhs.*, b2.lhs.*);
    try std.testing.expectEqual(b1.rhs.*, b2.rhs.*);
}

const Value = union(enum) {
    const Result = union(enum) {
        Integer: i32,
        Bool: bool,
    };

    Constant: i32,
    Bool: bool,
    Die: Die,
    Unary: Unary,
    Binary: Binary,

    pub fn get(self: Value, random: std.rand.Random) Result {
        return switch (self) {
            .Constant => |c| .{ .Integer = c },
            .Bool => |b| .{ .Bool = b },
            .Die => |die| .{ .Integer = random.intRangeAtMost(i32, 1, die.d) },
            .Unary => |op| op.get(random),
            .Binary => |op| op.get(random),
        };
    }

    /// For operators, frees all operand pointers.  Must be passed the same allocator that was used to allocate the Value tree.
    pub fn deinit(self: *Value, allocator: std.mem.Allocator) void {
        switch (self.*) {
            .Constant, .Bool, .Die => {},
            .Unary => |*op| op.*.deinit(allocator),
            .Binary => |*op| op.*.deinit(allocator),
        }
    }
};

test "get Constant" {
    var prng = std.rand.DefaultPrng.init(0);
    var rand = prng.random();
    const value = Value{ .Constant = 5 };
    try std.testing.expectEqual(value.get(rand).Integer, 5);
}

test "Binary Operation from json" {
    var j =
        \\{
        \\  "op": "multiply",
        \\  "lhs": 10,
        \\  "rhs": 5
        \\}
    ;
    var ten = Value{ .Constant = 10 };
    var five = Value{ .Constant = 5 };
    const expected = Binary{ .op = .multiply, .lhs = &ten, .rhs = &five };
    const options = std.json.ParseOptions{ .allocator = std.testing.allocator };
    const actual = try std.json.parse(Binary, &std.json.TokenStream.init(j), options);
    defer std.json.parseFree(Binary, actual, options);
    try std.testing.expectEqual(expected.op, actual.op);
    try std.testing.expectEqual(actual.lhs.*.Constant, 10);
    try std.testing.expectEqual(actual.rhs.*.Constant, 5);
    try std.testing.expectEqual(expected.lhs.*.Constant, actual.lhs.*.Constant);
    try std.testing.expectEqual(expected.rhs.*.Constant, actual.rhs.*.Constant);
}

test "Die from json" {
    var j =
        \\{
        \\  "d": 10
        \\}
    ;
    try std.testing.expectEqual(try std.json.parse(Die, &std.json.TokenStream.init(j), .{}), d10);
}

test "Die to json" {
    var out_buf: [9]u8 = undefined;
    var slice_stream = std.io.fixedBufferStream(&out_buf);
    const out = slice_stream.writer();
    try std.json.stringify(d10, .{}, out);
    try std.testing.expect(std.mem.eql(u8, "{\"d\":10}", slice_stream.getWritten()));
}

test "Dice from json" {
    var j =
        \\[
        \\  { "d": 10 },
        \\  { "d": 20 },
        \\  { "d":  6 }
        \\]
    ;
    const options = std.json.ParseOptions{ .allocator = std.testing.allocator };
    const dice = try std.json.parse([]Die, &std.json.TokenStream.init(j), options);
    defer std.json.parseFree([]Die, dice, options);
    try std.testing.expectEqual(dice[0], d10);
    try std.testing.expectEqual(dice[1], d20);
    try std.testing.expectEqual(dice[2], d6);
}

test "Dice to json" {
    var out_buf: [100]u8 = undefined;
    var slice_stream = std.io.fixedBufferStream(&out_buf);
    const out = slice_stream.writer();
    try std.json.stringify([_]Die{
        d10,
        d20,
        d6,
    }, .{}, out);
    try std.testing.expect(std.mem.eql(u8, "[{\"d\":10},{\"d\":20},{\"d\":6}]", slice_stream.getWritten()));
}

// TODO: Write a TokenStream iterator type that scans the input string and yields
// a sequence of Numbers, Operators, OpenParen, and CloseParen.
// Numbers: '0'...'9' +
// Operators:
//   '+' : binary
//   '-' : unary or binary
//   '*' : binary
//   '/' : binary
//   'v' : min
//   '^' : max
//   'd' : unary or binary
const Token = union(enum) {
    Number: i32,
    Operation: enum { add, subtract, multiply, divide, min, max, dice },
    OpenParen: struct {},
    CloseParen: struct {},
};

// TODO: Make this lazy?
const TokenStream = struct {
    pub fn init(s: []const u8) !TokenStream {}
    pub fn next(self: *TokenStream) ?Token {}
};

const ParseSlice = struct {
    start: usize,
    end: ?usize = null,
};

const ParseState = union(enum) {
    None: struct {},
    Dice: struct {
        count: usize,
        faces: usize, // start index
    },
    Number: usize, // start index
};

pub fn expandDice(allocator: std.mem.Allocator, count: usize, faces: i32) !Value {
    var die = Value{ .Die = .{ .d = faces } };
    var value = die;
    var i: usize = 1;
    while (i < count) {
        const lhs = try allocator.create(Value);
        const rhs = try allocator.create(Value);
        lhs.* = value;
        rhs.* = die;
        value = Value{ .Binary = .{
            .op = .add,
            .lhs = lhs,
            .rhs = rhs,
        } };
        i += 1;
    }
    return value;
}

pub fn parse(allocator: std.mem.Allocator, s: []const u8) !?Value {
    const Stack = std.SinglyLinkedList(ParseState);
    const Node = Stack.Node;

    var arena = std.heap.ArenaAllocator.init(allocator);
    defer arena.deinit();
    const stack_allocator = arena.allocator();

    var value: ?Value = null;
    var stack = Stack{};
    {
        const node = try stack_allocator.create(Node);
        node.* = Node{ .data = ParseState{ .None = .{} } };
        stack.prepend(node);
    }

    for (s) |c, i| {
        switch (stack.first.?.*.data) {
            .None => switch (c) {
                '0'...'9' => {
                    const node = try stack_allocator.create(Node);
                    node.* = Node{ .data = ParseState{ .Number = i } };
                    stack.prepend(node);
                },
                'd' => {
                    const node = try stack_allocator.create(Node);
                    node.* = Node{ .data = ParseState{ .Dice = .{ .count = 1, .faces = i + 1 } } };
                    stack.prepend(node);
                },
                ' ' => {},
                else => unreachable,
            },
            .Dice => |dice| switch (c) {
                // As long as we're seeing digits, just keep going
                '0'...'9' => {},
                'd' => unreachable,
                ' ' => {
                    value = try expandDice(allocator, dice.count, try std.fmt.parseInt(i32, s[dice.faces..i], 10));
                    stack_allocator.destroy(stack.popFirst().?);
                },
                else => unreachable,
            },
            .Number => |start| switch (c) {
                // As long as we're seeing digits, just keep going
                '0'...'9' => {},
                // If we see a 'd', parse the number into a number of dice and initialize a new Dice state
                'd' => {
                    stack_allocator.destroy(stack.popFirst().?);
                    const node = try stack_allocator.create(Node);
                    node.* = Node{ .data = ParseState{ .Dice = .{ .count = try std.fmt.parseInt(usize, s[start..i], 10), .faces = i + 1 } } };
                    stack.prepend(node);
                },
                ' ' => {
                    const number = try std.fmt.parseInt(i32, s[start..i], 10);
                    value = Value{ .Constant = number };
                    stack_allocator.destroy(stack.popFirst().?);
                },
                else => unreachable,
            },
        }
    }
    if (stack.popFirst()) |first| {
        value = switch (first.*.data) {
            .Dice => |dice| try expandDice(allocator, dice.count, try std.fmt.parseInt(i32, s[dice.faces..], 10)),
            .Number => |number| Value{ .Constant = try std.fmt.parseInt(i32, s[number..], 10) },
            else => unreachable,
        };
        stack_allocator.destroy(first);
    }
    return value;
}

test "Parse number" {
    var prng = std.rand.DefaultPrng.init(0);
    const rand = prng.random();
    const value = try parse(std.testing.allocator, "10");
    const result = value.?.get(rand).Integer;
    try std.testing.expectEqual(value.?, Value{ .Constant = 10 });
    try std.testing.expectEqual(result, 10);
}

// TODO: Implement parsing of binary operations
test "Parse equations with numbers only" {
    var prng = std.rand.DefaultPrng.init(0);
    const rand = prng.random();
    var equation = try parse(std.testing.allocator, "1+1");
    defer equation.?.deinit(std.testing.allocator);
    const result = equation.?.get(rand).Integer;
    try std.testing.expectEqual(equation.?.Binary.op, .add);
    try std.testing.expectEqual(equation.?.Binary.lhs.*.Constant, 1);
    try std.testing.expectEqual(equation.?.Binary.rhs.*.Constant, 1);
    try std.testing.expectEqual(result, 2);
}

test "Parse dice" {
    var prng = std.rand.DefaultPrng.init(0);
    const rand = prng.random();
    const d_4 = try parse(std.testing.allocator, "d4");
    const one_d_4 = try parse(std.testing.allocator, "1d4");
    var two_d_4 = try parse(std.testing.allocator, "2d4");
    defer two_d_4.?.deinit(std.testing.allocator);
    var four_d_10 = try parse(std.testing.allocator, "4d10");
    defer four_d_10.?.deinit(std.testing.allocator);
    var i: usize = 0;
    while (i < 10_000) {
        {
            const result = d_4.?.get(rand).Integer;
            try std.testing.expectEqual(d_4.?, Value{ .Die = .{ .d = 4 } });
            try std.testing.expect(1 <= result);
            try std.testing.expect(result <= 4);
        }
        {
            const result = one_d_4.?.get(rand).Integer;
            try std.testing.expectEqual(one_d_4.?, Value{ .Die = .{ .d = 4 } });
            try std.testing.expect(1 <= result);
            try std.testing.expect(result <= 4);
        }
        {
            const result = two_d_4.?.get(rand).Integer;
            // d4 + d4
            try std.testing.expectEqual(two_d_4.?.Binary.op, .add);
            try std.testing.expectEqual(two_d_4.?.Binary.lhs.*.Die, d4);
            try std.testing.expectEqual(two_d_4.?.Binary.rhs.*.Die, d4);
            try std.testing.expect(2 <= result);
            try std.testing.expect(result <= 8);
        }
        {
            const result = four_d_10.?.get(rand).Integer;
            // ((d10 + d10) + d10) + d10
            //                     ^
            try std.testing.expectEqual(four_d_10.?.Binary.op, .add);
            // ((d10 + d10) + d10) + d10
            //                       ^^^
            try std.testing.expectEqual(four_d_10.?.Binary.rhs.*.Die, d10);
            // ((d10 + d10) + d10) + d10
            //              ^
            try std.testing.expectEqual(four_d_10.?.Binary.lhs.*.Binary.op, .add);
            // ((d10 + d10) + d10) + d10
            //                ^^^
            try std.testing.expectEqual(four_d_10.?.Binary.lhs.*.Binary.rhs.*.Die, d10);
            // ((d10 + d10) + d10) + d10
            //       ^
            try std.testing.expectEqual(four_d_10.?.Binary.lhs.*.Binary.lhs.*.Binary.op, .add);
            // ((d10 + d10) + d10) + d10
            //   ^^^
            try std.testing.expectEqual(four_d_10.?.Binary.lhs.*.Binary.lhs.*.Binary.lhs.*.Die, d10);
            // ((d10 + d10) + d10) + d10
            //         ^^^
            try std.testing.expectEqual(four_d_10.?.Binary.lhs.*.Binary.lhs.*.Binary.rhs.*.Die, d10);
            // Rolling 4d10 results in a value between 4 and 40
            try std.testing.expect(4 <= result);
            try std.testing.expect(result <= 40);
        }
        i += 1;
    }
}

// test "Parse dice + number" {
//     try std.testing.expectEqual(parse(std.testing.allocator, "1d4 + 1"), ?Value{ .Binary = .{ .op = .add, .lhs = &.{ .Die = .{ .d = 4 } }, .lhs = &.{ .Constant = 1 } } });
// }
