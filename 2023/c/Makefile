
all: build \
	build/bin \
	build/bin/day1_q1 \
	build/bin/day1_q2 \
	build/bin/day2_q1 \
	build/bin/day2_q2 \
	build/bin/day3_q1 \
	build/bin/day3_q2 \
	build/bin/day4_q1 \
	build/bin/day4_q2 \
	build/bin/day5_q1 

build:
	mkdir -p build

build/bin: build
	mkdir -p build/bin

build/%.o: %.c
	gcc -o $@ $^ -c

build/bin/day%: build/day%.o build/aoc_strings.o
	gcc -o $@ $^ 
