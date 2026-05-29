gcc -std=c90 -Wall -Wpedantic 0001219659.c -o file -lm
valgrind ./file input1.txt
valgrind ./file input2.txt
valgrind ./file input3.txt