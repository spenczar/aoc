def main():
    f = open("inputs/1/input", "rb")
    prev_window = (0, 0, 0)
    cur_window = (0, 0, 0)
    n = 0
    i = 0
    for line in f.readlines():
        line_int = int(line)
        cur_window = (line_int, cur_window[0], cur_window[1])
        if i >= 3 and sum(cur_window) > sum(prev_window):
            n += 1
        prev_window = cur_window
        i += 1
    print(n)


if __name__ == "__main__":
    main()
