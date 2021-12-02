const std = @import("std");

const Submarine = struct {
    x: u32,
    y: u32,

    pub fn init() Submarine {
        return Submarine{ .x = 0, .y = 0 };
    }

    pub fn scanFile(self: *Submarine, fname: []const u8) !void {
        const f = try std.fs.cwd().openFile(fname, std.fs.File.OpenFlags{ .read = true });
        defer f.close();

        var stream = std.io.bufferedReader(f.reader()).reader();
        var buf: [1024]u8 = undefined;

        while (try stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
            try self.readLine(line);
        }
    }

    pub fn readLine(self: *Submarine, line: []const u8) !void {
        const instr = try Instruction.parseLine(line);
        switch (instr.dir) {
            Direction.forward => {
                self.x += instr.amt;
            },
            Direction.up => {
                self.y -= instr.amt;
            },
            Direction.down => {
                self.y += instr.amt;
            },
            Direction.invalid => {
                unreachable;
            },
        }
    }
};

const Direction = enum {
    forward,
    down,
    up,
    invalid,

    pub fn strlen(self: Direction) usize {
        return switch (self) {
            Direction.forward => "forward ".len,
            Direction.down => "down ".len,
            Direction.up => "up ".len,
            Direction.invalid => 0,
        };
    }
};

const Instruction = struct {
    dir: Direction,
    amt: u32,

    pub fn parseLine(line: []const u8) !Instruction {
        var instr = Instruction{
            .dir = switch (line[0]) {
                'f' => Direction.forward,
                'd' => Direction.down,
                'u' => Direction.up,
                else => Direction.invalid,
            },
            .amt = undefined,
        };

        if (instr.dir == Direction.invalid) {
            std.debug.print("broken line: {s}  line[0]={c}\n", .{ line, line[0] });
            return error.InvalidDirection;
        }

        // Extract the string part, which should
        const amtStr = line[instr.dir.strlen()..];

        instr.amt = try std.fmt.parseInt(u32, amtStr, 10);

        return instr;
    }
};

pub fn main() !void {
    const fname = "inputs/2/input";
    var sub = Submarine.init();
    try sub.scanFile(fname);

    const result = sub.x * sub.y;
    std.debug.print("{d}\n", .{result});
}
