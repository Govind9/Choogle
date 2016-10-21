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

using namespace std;

enum object_type {
	File,
	Dir,
	Hidden,
	Lnk,
	Other,
};

struct File {
	string path;
	bool is_modified;
};

void display_paths();

void get_all_file_paths(string);

bool is_modified(struct File*);

void mining();

inline bool is_symbol(char);

void index(string);
