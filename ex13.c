#include <stdio.h>

char upcase(char c) {
	if (97 <= c && c <= 122) {
		return c - 32;
	} else {
		return c;
	}
}

void print_phrase(char phrase[])
{
	int i;
	char letter;

	for(i = 0; letter = upcase(phrase[i]), letter != '\0'; i++) {
		switch(letter) {
			case 'A':
				printf("%d: 'A'\n", i);
				break;

			case 'E':
				printf("%d: 'E'\n", i);
				break;

			case 'I':
				printf("%d: 'I'\n", i);
				break;

			case 'O':
				printf("%d: 'O'\n", i);
				break;

			case 'U':
				printf("%d: 'U'\n", i);
				break;

			case 'Y':
				if(i > 2) {
					// it's only somtimes Y
					printf("%d: 'Y'\n", i);
					break;
				}

			default:
				printf("%d: '%c' is not a vowel\n", i, letter);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("ERROR: You need at least one argument.\n");
		// this is how you abort a program
		return 1;
	}

	int i;

	for (i = 1; i < argc; i++) {
		printf("------\n");
		printf("Arg %d:\n", i);
		printf("------\n");
		print_phrase(argv[i]);
	}

	return 0;
}
