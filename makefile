all:prog

prog:main.c sqlite3.c
	gcc -std=c99 main.c sqlite3.c -o prog -lpthread -ldl
main.c:

sqlite3.c:
