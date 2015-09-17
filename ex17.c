#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

/*
 * Define the Address struct using MAX_DATA to define
 * fixed size string fields `name` and `email`.
 */
struct Address {
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

/*
 * Define the Database struct with fixed number of rows
 * derived from MAX_ROWS.
 */
struct Database {
	struct Address rows[MAX_ROWS];
};

/*
 * Define the Connection struct which contains a pointer
 * to a FILE struct and a pointer to a Database struct.
 */
struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close(struct Connection *conn);

/*
 * Print `message` and terminate the program.
 */
void die(const char *message, struct Connection *conn)
{
	// Some functions will set the errno global variable.
	// If so, use perror.
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	if(conn) {
		Database_close(conn);
	}

	exit(1);
}

/*
 * Pretty print `addr`.
 */
void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

/*
 * Read one Database-sized item from the conn->file stream into conn->db.
 */
void Database_load(struct Connection *conn)
{
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die("Failed to load database.", conn);
}

/*
 * Create a new connection to database at `filename` in given `mode`.
 */
struct Connection *Database_open(const char *filename, char mode)
{
	// Allocate enough bytes for a new Connection on the heap.
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory error", conn);

	// Allocate enough bytes for a new Database on the heap and store
	// and pointer in the freshly allocated Connection.
	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) die("Memory error", conn);

	if(mode == 'c') {
		// If in 'c' (create) mode, open the file in write-only mode.
		// This will truncate the file to zero.
		conn->file = fopen(filename, "w");
	} else {
		// Otherwise, open in read-write mode.
		conn->file = fopen(filename, "r+");

		if(conn->file) {
			Database_load(conn);
		}
	}
	
	if(!conn->file) die("Failed to open the file", conn);

	return conn;
}

/*
 * Close the stream in `conn->file` and free memory allocated for `conn->db`
 * and `conn` itself.
 */
void Database_close(struct Connection *conn)
{
	if(conn) {
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
		free(conn);
	}
}

/*
 * Flush contents of `conn->db` to `conn->file`.
 */
void Database_write(struct Connection *conn)
{
	// Rewind the file stream first.
	rewind(conn->file);

	// Send bytes from conn->db to conn->file stream.
	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc !=1) die("Failed to write database.", conn);

	// Flush conn->file stream to the filesystem.
	rc = fflush(conn->file);
	if(rc == -1) die("Cannot flush database.", conn);
}

/*
 * Initialize contents of `conn->db`.
 */
void Database_create(struct Connection *conn)
{
	int i = 0;

	for(i = 0; i < MAX_ROWS; i++) {
		// Make a prototype to initialize it.
		struct Address addr = {.id = i, .set = 0};
		// Then just assign it.
		conn->db->rows[i] = addr;
	}
}

/*
 * Set `name` and `email` for record `id`.
 */
void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Already set, delete it first", conn);

	addr->set = 1;
	// WARNING: bug, read the "How To Break It" and fix this
	char *res = strncpy(addr->name, name, MAX_DATA);
	res[MAX_DATA - 1] = '\0';
	// demonstrate strncpy bug
	if(!res) die("Name copy failed", conn);

	res = strncpy(addr->email, email, MAX_DATA);
	if(!res) die("Email copy failed", conn);
}

/*
 * Print Address at `id`.
 */
void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(addr);
	} else {
		die("ID is not set", conn);
	}
}

/*
 * Re-initialize Address at `id` to blank slate.
 */
void Database_delete(struct Connection *conn, int id)
{
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

/*
 * Print all non-blank addresses in conn->db.
 */
void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	for(i = 0; i < MAX_ROWS; i++) {
		struct Address *cur = &db->rows[i];

		if(cur->set) {
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	if(argc > 3) id = atoi(argv[3]);
	if(id >= MAX_ROWS) die("There aren't that many records.", conn);

	switch(action) {
		case 'c':
			Database_create(conn);
			Database_write(conn);
			break;

		case 'g':
			if (argc != 4) die("Need an id to get", conn);

			Database_get(conn, id);
			break;

		case 's':
			if(argc != 6) die("Need id, name, email to set", conn);

			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;

		case 'd':
			if (argc != 4) die("Need id to delete", conn);

			Database_delete(conn, id);
			Database_write(conn);
			break;

		case 'l':
			Database_list(conn);
			break;

		default:
			die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
	}

	Database_close(conn);

	return 0;
}

