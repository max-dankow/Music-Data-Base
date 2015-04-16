all:prog

prog:main.o sqlite3.o shell.o
	gcc main.c sqlite3.c -o prog -lpthread -ldl
main.c:

sqlite3.c:
