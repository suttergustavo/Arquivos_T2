all:
	gcc src/*.c -o prog -g -std=c99 -I./include -Wall
	rm xurbo_*
run:
	./prog
debug:
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./prog