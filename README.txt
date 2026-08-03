Name: Noah Greer
BlazerId: njgreer
Project: HW3

How to compile: make

How to run: ./hw3

Example: ./hw3 testfiles

What the program does:
     Checks that a directory name was given on the command line.
     Checks that the directory actually exists.
     Goes through every file in the directory skiping subdirectories.
     For each file creates a child process that prints:
        - the file name
        - the file size in bytes using stat()
        - the number of words in the file but only if the file ends in .txt
     The parent process waits for every child process to finish before exiting.

Edge cases handled:
     No command line argument given prints usage message and exits.
     Directory does not exist prints Error: Directory not found. and exits.
     Subdirectories inside the given directory are skipped
     Empty files word count comes back as 0 no crash.
     Files that fail to open prints an error message and skips that file instead of crashing.
     fork() failure prints an error message and exits the program.

Testing notes:
     Tested with a folder containing a mix of .txt files and non .txt files.
     Tested with an empty .txt file.
     Tested with a directory that does not exist.
     Tested running the program with no arguments.   All looked as expected.