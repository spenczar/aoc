def main():
    f = open("inputs/1/input", "rb")
    prev = None
    n = 0
    for line in f.readlines():
        line_int = int(line)
        if prev is not None and line_int > prev:
            n += 1
        prev = line_int
    print(n)

if __name__ == "__main__":
    main()
