# CS 5600 Assignment 1

## Overview

This assignment is a (re-)introduction to memory-mapped file I/O, the ELF header
format, and C development on Ubuntu.

To complete it, implement the two functions in `answer/answer.c` according to
the specifications in `answer/answer.h`, so that they pass all of the provided
tests without leaking memory as detected by `valgrind`.

To make this assignment easier, `evaluate.sh` has been provided. Running it will
verify dependencies are installed, generate a Makefile, build the project, run
the tests under valgrind, and give basic pass/fail feedback. Note that running
it on the initial project will produce a large quantity of failure text!

## Goals

* In `answer/answer.c`:
  * Implement `ElfData getELFData(const char *)` using memory-mapped file I/O
  * Implement `void destroyELFData(ElfData)`
* Ensure that your solution passes all of the included tests
* Ensure that your solution does not leak any memory

### Requirements

* Document any interesting/difficult sections of your code
  * Git commit comments will count towards your documentation grade if present
* Use a consistent code style
  * This project was formatted using
    `clang-format -i -style="{BasedOnStyle: Google, IndentWidth: 4}" *`
  * Feel free to select a different style or `sudo apt install clang-format` to
    install `clang-format`
* Your code should compile without compiler warnings or errors
  * Do not disable compiler warnings without documenting why there was no
    other alternative
* Do not alter any of the provided files other than `answer/answer.c`
  * Doing so may invalidate your grade
  * `evaluate.sh` will warn you if it detects that files have been modified

### Grading

* 50% - functional tests passing
* 20% - no memory leaks during testing
* 20% - documentation quality
* 10% - consistent code style

## References

### Memory-mapped I/O

* [mmap() man page](http://man7.org/linux/man-pages/man2/mmap.2.html)
* [Wikipedia: Memory-mapped file](https://en.wikipedia.org/wiki/Memory-mapped_file)

### ELF header format

* [ELF man page](http://man7.org/linux/man-pages/man5/elf.5.html)
* [Wikipedia: ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)

## WSL Support

The Windows Subsystem for Linux (WSL) allows you to install Ubuntu 20.04.1 LTS
and execute it alongside your Windows kernel in order to run standard ELF
binaries. WSL has some tradeoffs but is capable of handling this project.

You will need to install and configure WSL and Ubuntu 20.04.1 LTS yourself. You should be able to install it
from the microsoft store.  WSL will not work if you have compression and encryption enabled on your system. 
In order to fix this, do the following:

In PowerShell (Run As Administrator):
```shell
fsutil behavior set disableencryption 1
fsutil behavior set disablecompression 1
```

You'll have to reboot after this

From your home directory in powershell:

```shell
compact /U /S /I
```

CLion should also let you pick WSL as your tool chain after this for building, while you should be able to run
./evaluate.sh from a WSL command line.
