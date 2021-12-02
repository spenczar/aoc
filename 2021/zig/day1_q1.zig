const std = @import("std");

const DiffChecker = struct {
    first_iter: bool,
    last_val: i32,
    n_increase: u32,

    pub fn init() DiffChecker {
        return DiffChecker{
            .first_iter = true,
            .last_val = 0,
            .n_increase = 0,
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

    pub fn readLine(self: *DiffChecker, line: []const u8) !void {
        const val = try std.fmt.parseInt(i32, line, 10);

        if (!self.first_iter and val > self.last_val) {
            self.n_increase += 1;
        }
        if (self.first_iter) {
            self.first_iter = false;
        }
        self.last_val = val;
    }
};

pub fn main() !void {
    const fname = "inputs/1/input";

    var dc = DiffChecker.init();
    try dc.scanFile(fname);
    std.debug.print("{d}\n", .{dc.n_increase});
}
