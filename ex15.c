#include <stdio.h>

void print_years_alive(char **name, int *age)
{
	printf("%s has %d years alive.\n", *name, *age);
}

void print_names_and_ages(char **names, int *ages, int count)
{
	int i = 0;
	for(i = 0; i < count; i++) {
		printf("%s has %d years alive.\n", names[i], ages[i]);
	}
}

int main(int argc, char *argv[])
{
	// create two arrays we care about
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {
		"Alan", "Frank",
		"Mary", "John", "Lisa"
	};

	// safely get the size of ages
	int count = sizeof(ages) / sizeof(int);
	int i = 0;

	// first way using indexing
	for(i = count -1; i >= 0; i--) {
		print_years_alive(&names[i], &ages[i]);
	}

	printf("---\n");

	print_names_and_ages(names, ages, count);

	printf("---\n");

	// setup our pointers to the start of the arrays
	int *cur_age = ages;
	char **cur_name = names;

	// second way of using pointers
	for(i = count - 1; i >= 0; i--) {
		printf("%s is %d years old.\n",
				*(cur_name + i), *(cur_age+i));
	}

	printf("---\n");

	// third way, pointers are just arrays
	for(i = count - 1; i >= 0; i--) {
		printf("%s is %d years old again.\n",
				cur_name[i], cur_age[i]);
	}

	printf("---\n");

	// fourth way with points in a stupid complex way
	for(cur_name = names + count - 1, cur_age = ages + count - 1;
			(cur_age - ages) >= 0;
			cur_name--, cur_age--)
	{
		printf("%s lived %d years so far.\n",
				*cur_name, *cur_age);
	}

	printf("---\n");

	for(i = 0; i < count; i++) {
		printf("%p: %s\n", &names[i], names[i]);
		printf("%p: %d\n", &ages[i], ages[i]);
	}

	return 0;
}
