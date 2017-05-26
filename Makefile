#Sourced: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
CC = gcc
CFLAGS = -Wall -I.

all: test

knn: knn.c
	$(CC) knn.c terminal_user_input.c $(CFLAGS) -DNDEBUG -o knn

test: knn.c
	$(CC) terminal_user_input.c tests.c $(CFLAGS) -o test_knn
