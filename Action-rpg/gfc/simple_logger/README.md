# simple_logger
A simple, lightweight log-to-file option for C and C++ projects.

## Features
* No dependencies beyond standard C library
* just drop the .c and .h files into your project and get started
* automatic file and line numbers added to log messages

## Usage

1) at the start of your program run
```c
init_logger("YOUR LOG FILE GOES HERE");
```
    
2) frequently call and before program exit
```c
slog_sync();
```
(Such as every game or program loop or in its own thread)

3) Call
```c
slog(...);
```
whenever you want to write something to file and standard out
