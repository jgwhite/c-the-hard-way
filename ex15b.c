#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 1;

	for (i = 1; i < argc; i++) {
		printf("Arg %d: %s (%p)\n",
				i,
				*(argv + i),
				&argv[i]);
	}

	return 0;
}
