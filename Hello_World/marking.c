#include <unistd.h> // for write() & macros

int main(void)
{
    // Define both strings
    const char *str_stdout = "Hello, World!\n";
    const char *str_stderr = "Hello, Error!\n";

    // Write both messages on STDOUT (1) & STDERR (2) respectively
    write(STDOUT_FILENO, str_stdout, 14);
    write(STDERR_FILENO, str_stderr, 14);

    // Exit successfully
    return 0;
}
