#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Make sure at least one argument is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 1; i < argc; ++i) {
        const char *filename = argv[i];
        int fd = open(filename, O_RDONLY);
    
        if (fd == -1) {
            /*if (errno == ENOENT) {
                fprintf(stderr, "Error: File '%s' does not exist.\n", filename);
            } else {
                perror("open");
            }*/
	    perror("open");
            exit(EXIT_FAILURE);
        }
    
        printf("File '%s' exists.\n", filename);
        close(fd);
    }

    return 0;
}
