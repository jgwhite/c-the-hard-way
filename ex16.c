// gives us printf
#include <stdio.h>
// gives us assert
#include <assert.h>
// gives us malloc and free
#include <stdlib.h>
// gives us strdup
#include <string.h>

// Defines the Person struct
struct Person {
	// The `name` field, a string
	char *name;
	// Three int fields
	int age;
	int height;
	int weight;
};

// Person_create function
struct Person *Person_create(char *name, int age, int height, int weight)
{
	// Allocate enough memory to hold the new person
	struct Person *who = malloc(sizeof(struct Person));
	// Assert the memory was definitely allocated and
	// we have a valid pointer
	assert(who != NULL);

	// Copy the name arguments and write it to the person's name field
	who->name = strdup(name);
	// Write age, height and weight to their respective fields
	who->age = age;
	who->height = height;
	who->weight = weight;

	return who;
}

// Person_destroy function
void Person_destroy(struct Person *who)
{
	// Assert we have a valid pointer
	assert(who != NULL);

	// Free the name string
	free(who->name);
	// Free the person struct
	free(who);
}

// Person_print function
void Person_print(struct Person *who)
{
	printf("Name: %s\n", who->name);
	printf("\tAge: %d\n", who->age);
	printf("\tHeight: %d\n", who->height);
	printf("\tWeight: %d\n", who->weight);
}

int main(int argc, char *argv[])
{
	// make two people structures
	struct Person *joe = Person_create(
			"Joe Alex", 32, 64, 100);

	struct Person *frank = Person_create(
			"Frank Blank", 20, 72, 180);

	// print them out and where they are in memory
	printf("Joe is at memory location %p:\n", joe);
	Person_print(joe);

	printf("Frank is at memory location %p:\n", frank);
	Person_print(frank);

	// make everyone age 20 years and print them again
	joe->age += 20;
	joe->height -= 2;
	joe->weight += 40;
	Person_print(joe);

	frank->age += 20;
	frank->weight += 20;
	Person_print(frank);

	// destroy them both so we clean up
	Person_destroy(joe);
	Person_destroy(frank);

	return 0;
}
