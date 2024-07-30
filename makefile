all:run

BIN_DIR=bin
EXEC_NAME=SpaceShooter
EXEC=$(BIN_DIR)/$(EXEC_NAME)

SRC_DIR=src
LANG=cpp
SRC=$(wildcard $(SRC_DIR)/*.$(LANG))

CC=g++
CFLAGS=-Wall -Wextra -pedantic -std=c++23
LDFLAGS=-lraylib

bin_dir:
	mkdir -p $(BIN_DIR)

build:bin_dir
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LDFLAGS)

run:build
	$(EXEC)
