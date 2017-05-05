all:
	gcc src/*.c -o prog -g -std=c99 -I./include -Wall
clean:
	rm out
run:
	./prog
debug:
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./prog