#include <stdio.h>
// write() lib
#include <unistd.h>

int main()
{
	// msg we want to display
	char hello[12] = "Hello Word!\n";
	char hello_error[14] = "Hello, Error!\n";

	/*
	 * syst call : write(fd, content, cnt_size)
	 * 	- fd (File Descriptors) :
	 * 		= 0 -> stdin (read input = input stream)
	 * 		= 1 -> stdout(write value = output stream)
	 * 		= 2 -> stderr(write error = output error stream)
	 *	- cnt_size : need <= INT_MAX
	 */
	// normal case
	write(1, hello, 12);
	// error  case 
	write(2, hello_error, 14);
}
