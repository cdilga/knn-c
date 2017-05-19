#Sourced: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
CC = gcc
CFLAGS = -Wall -I.

all: knn

knn: knn.c
	$(CC) knn.c terminal_user_input.c $(CFLAGS) -o MusicPlayer
