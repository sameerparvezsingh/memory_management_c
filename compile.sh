#!/bin/bash

# Compile memory manager
gcc -c memory_manager.c -o memory_manager.o

# Compile main program
gcc -c main.c -o main.o

# Link and create executable
gcc main.o memory_manager.o -o memory_demo

# Run the program
./memory_demo