all: minimax
build: minimax.c
	gcc -g3 -o minimax minimax.c
clean:
	rm minimax
