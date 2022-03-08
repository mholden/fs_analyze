CC=gcc
INCLUDES=-I./ext4

all: fs_analyze

fs_analyze: fs_analyze.o ext4_analyze.o
	$(CC) fs_analyze.o ext4_analyze.o -o fs_analyze

ext4_analyze: fs_analyze.o ext4_analyze.o
	$(CC) fs_analyze.o ext4_analyze.o -o fs_analyze

fs_analyze.o: fs_analyze.c
	$(CC) $(INCLUDES) -c fs_analyze.c

ext4_analyze.o: ext4/ext4_analyze.c
	$(CC) -c ext4/ext4_analyze.c

clean:
	rm *o fs_analyze

install:
	cp fs_analyze $(HOME)/devel/install/

.PHONY: all clean install
