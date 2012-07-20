#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char ** argv) {
	int i, t;
	
	// file descriptors
	int * file_ds;
	int out_ds;
	char buf;
	ssize_t n;

	file_ds = (int *) malloc ((argc-1)*sizeof(int));
	
	// llenar el arreglo de file descriptors (abrir archivos)
	for (i = 1; i < argc; i++) 
		file_ds[i-1] = open(argv[i], O_RDONLY);

	out_ds = creat("output", S_IRWXU | S_IRUSR | S_IRWXG);
	
	for (i = 0, t = 0; n > 0; t = (++i) % (argc-1)) {
		printf("%d", t);
		n = read (file_ds[t], (char *) &buf, 1);
		printf("%c ", buf);
		if (t == 2)
			printf("\n");
	}
}
