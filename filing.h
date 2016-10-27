#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define THE_INDEX_FILE "Index.txt"
#define THE_FILES_FILE "Files.txt"

using namespace std;

enum object_type {
	File,
	Dir,
	Hidden,
	Lnk,
	Other,
};

struct File {
	int id;
	string name;
	string path;
	bool is_modified;
};

struct Occurence {
	map <struct File*, int> count_hash;
};

void clean_sweep();

void deserialize();

void serialize();

inline string to_string(int);

void display_paths();

void get_all_file_paths(string);

bool is_modified(struct File*);

void mining();

inline bool is_symbol(char);

void index(string, struct File*);

void display_index();

inline bool is_not_a_word(string);

bool is_a_stop_word(string);
