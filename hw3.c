/* Name: Noah Greer
BlazerId: njgreer
Project #: HW3
To compile: make
To run: ./hw3 <directory_name> */

#include <stdio.h>      // printf, fopen, fclose, 
#include <stdlib.h>     // exit()
#include <string.h>     // strcmp, strlen, strstr
#include <dirent.h>     // opendir, readdir, closedir
#include <sys/stat.h>   // stat()
#include <sys/wait.h>   // wait()
#include <unistd.h>     // fork()

// This function checks if a filename ends in ".txt"
// this is to decide whether should count words in the file.

int isTextFile(const char *filename) {
    int len = strlen(filename);
    if (len < 4) {
        return 0; // too short to end in ".txt"
    }
    // Compare the last 4 characters of the filename to ".txt"
    
    if (strcmp(filename + (len - 4), ".txt") == 0) {
        return 1; // yes, it is a .txt file
    }
    return 0; // no its not
}

// This function opens a file and counts how many words are in it.
// A "word" here is just a chunk of text separated by spaces / newlines / tabs.

int countWords(const char *fullPath) {
    FILE *fp = fopen(fullPath, "r");
    if (fp == NULL) {
        
        // Could not open the file
        
        printf("Error: could not open file %s for reading.\n", fullPath);
        return -1;
    }

    int wordCount = 0;
    
    // %s in fscanf skips whitespace and reads one word at a time.
    
    char word[1024];
    while (fscanf(fp, "%1023s", word) == 1) {
        wordCount++;
    }

    fclose(fp);
    return wordCount;
}

int main(int argc, char *argv[]) {

    // Check command line arguments
    if (argc != 2) {
        printf("Usage: %s <directory_name>\n", argv[0]);
        exit(1);
    }

    char *dirName = argv[1];

    // Try to open the directory

    DIR *dir = opendir(dirName);
    if (dir == NULL) {

        // opendir() returns NULL if the directory doesn't exist

        printf("Error: Directory not found.\n");
        exit(1);
    }

    // Go through every entry in the directory

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {

        // Skip the special entries "." and ".."

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Build the full path directory + / + filename

        char fullPath[2048];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirName, entry->d_name);

        // Use stat() to find out information about this entry,
        // including whether it is a regular file or a subdirectory.

        struct stat fileStat;
        if (stat(fullPath, &fileStat) == -1) {
            printf("Error: could not get info for %s\n", fullPath);
            continue; // skip this entry move to the next one
        }

        // only care about regular files not subdirectories.

        if (!S_ISREG(fileStat.st_mode)) {
            continue;
        }

        // Fork a child process for this file

        pid_t pid = fork();

        if (pid < 0) {
            // fork() failed
            printf("Error: fork failed.\n");
            exit(1);
        }
        else if (pid == 0) {

            // This code runs inside the CHILD process

            long fileSize = fileStat.st_size;

            if (isTextFile(entry->d_name)) {
                int words = countWords(fullPath);
                if (words >= 0) {
                    printf("File: %s | Size: %ld bytes | Words: %d\n",
                           entry->d_name, fileSize, words);
                }
            }
            else {

                // Not a .txt file so don't count words.

                printf("File: %s | Size: %ld bytes\n",
                       entry->d_name, fileSize);
            }

            // Child is done exit so it doesn't keep looping through readdir()

            exit(0);
        }
        // ---- If it gets there this is still the PARENT process
        // The parent just continues the while loop to look at the next file.
    }

    closedir(dir);

    // Parent waits for ALL child processes to finish

    while (wait(NULL) > 0) {
        // keep waiting until there are no more children left
    }

    return 0;
}
