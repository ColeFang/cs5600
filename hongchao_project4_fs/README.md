# CS 5600 Assignment 4

## Overview
In this assigment you will be implementing a file system, you will only be able to represent the disk in memory, you will not be writing to an actual disk.
You will be simulating INodes, directories, and files. There are a variety of features that are still missing from this assignment. The assignment is 
currently fully single threaded due to the fact that multithreaded is significantly harder to test. Also there is not actual planning of how to do this 
on disk, everything is held on memory
## Known Issues
1. This assignment is fully tested on Linux and Mac OSX(Intel and Apple Silicon). It may work on WSL but has not been fully tested.

## Implementation Details
Your sectors/blocks are 512kb, all pointers are 32 bit. This means that a single block can support 128 points.
You are required to support a minimum file size of 8MB. With 32 bit pointers and 512K blocks this will require doubly indirect blocks.

- answer/QUESTIONS.md
  - 4 questions about your code and the general problem must be answered
  
-answer/disk.c
  - These are the only required functions.
  - You will need more, documentation for these functions is in disk.h

## Building and Testing
cmake .
make

The above should set up all dependencies.

To run your program from command line, start in your project directory:

```
  cmake .
  make
  ./FileSystem
```

## Grading
  - 50% - Tests Passing
  - 25% - Code
  - 25% - Questions.md 
