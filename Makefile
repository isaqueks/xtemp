all: make

make:
	gcc main.c console.c cpu_temp.c -o xtemp -lm