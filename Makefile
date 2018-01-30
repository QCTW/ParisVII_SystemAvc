# the compiler: gcc for C program
CC=gcc

# the build target executable:
all:    clean main

main:
	$(CC) HsiehYK_TD1.c -o HsiehYK_TD1.exe


clean:
ifneq ("$(wildcard HsiehYK_TD1.exe)","")
	rm HsiehYK_TD1.exe
endif

