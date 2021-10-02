#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define BUFFER_SIZE 1024

int main (int argc, char* argv[]) {
	// open files and detect error
	FILE *src, *dest;
	src = fopen(argv[1], "r");
	if (src == NULL) {
		printf("Error: Could not open file '%s'.\n", argv[1]);
		exit(-1);
	}
	dest = fopen(argv[2], "w+");
	if (dest == NULL) {
		printf("Error: Could not open file '%s'.\n", argv[2]);
		fclose(src);
		exit(-1);
	}

	// start copying
	size_t read_in; // length of the read in string
	char buffer[BUFFER_SIZE];
	while (read_in = fread(buffer, 1, BUFFER_SIZE, src))
		fwrite(buffer, 1, read_in, dest);

	// close files
	fclose(src);
	fclose(dest);

	exit(0);
}
