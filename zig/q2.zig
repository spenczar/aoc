const std = @import("std");

const DiffChecker = struct {
    previous: [3]i32,
    current: [3]i32,
    n_seen: u32,
    n_increase: u32,

    pub fn init() DiffChecker {
        return DiffChecker{
            .previous = [3]i32{ 0, 0, 0 },
            .current = [3]i32{ 0, 0, 0 },
            .n_increase = 0,
            .n_seen = 0,
        };
    }

    pub fn scanFile(self: *DiffChecker, fname: []const u8) !void {
        const f = try std.fs.cwd().openFile(fname, std.fs.File.OpenFlags{ .read = true });
        defer f.close();

        var stream = std.io.bufferedReader(f.reader()).reader();
        var buf: [1024]u8 = undefined;

        while (try stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
            try self.readLine(line);
        }
    }

    inline fn currentSum(self: *DiffChecker) i32 {
        return self.current[0] + self.current[1] + self.current[2];
    }

    inline fn previousSum(self: *DiffChecker) i32 {
        return self.previous[0] + self.previous[1] + self.previous[2];
    }

    pub fn readLine(self: *DiffChecker, line: []const u8) !void {
        self.n_seen += 1;
        const val = try std.fmt.parseInt(i32, line, 10);

        self.current[0] = val;
        self.current[1] = self.previous[0];
        self.current[2] = self.previous[1];

        if (self.n_seen > 3 and self.currentSum() > self.previousSum()) {
            self.n_increase += 1;
        }

        self.previous[0] = self.current[0];
        self.previous[1] = self.current[1];
        self.previous[2] = self.current[2];
    }
};

pub fn main() !void {
    const fname = "inputs/1/input";

    var dc = DiffChecker.init();
    try dc.scanFile(fname);
    std.debug.print("{d}\n", .{dc.n_increase});
}
