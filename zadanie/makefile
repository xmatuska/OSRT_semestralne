# -*- Makefile -*-
CC=gcc
CFLAGS=-I.

target: dependencies
#	action
	
run: 
	gcc server.c -o server 
	gcc client1.c -o client1  
	gcc -o client2 client2.c -pthread 
	gcc client4.c -o client4
	
	clear
	 @echo	"Vitaj, toto je program na vypocet povrchu valca,\n"
	 @echo	"treba ma spustat v tomto poradi: \n\n"
	 @echo  "\t 1 - ./server\n"
	 @echo  "\t 2 - ./client1\n"
	 @echo  "\t 3 - ./client2\n"
	 @echo  "\t 4 - ./client4\n"