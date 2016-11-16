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
#include <stdlib.h>
#include <fcntl.h>

#define THE_INDEX_FILE "/.Index.txt"
#define THE_FILES_FILE "/.Files.txt"
#define ONE_MINUTE 60
#define FPW 10

//Color definitions
#define BLACK		"\033[0;30m"
#define RED		"\033[0;31m"
#define GREEN		"\033[0;32m"
#define YELLOW		"\033[0;33m"
#define BLUE		"\033[0;34m"
#define	MAGENTA		"\033[0;35m"
#define	CYAN		"\033[0;36m"
#define	WHITE		"\033[0;37m"
#define RED_BOLD	"\033[1;31m"
#define GREEN_BOLD	"\033[1;32m"
#define YELLOW_BOLD	"\033[1;33m"
#define BLUE_BOLD	"\033[1;34m"
#define MAGENTA_BOLD	"\033[1;35m"
#define	CYAN_BOLD	"\033[1;36m"
#define	WHITE_BOLD	"\033[1;37m"
#define END_COLOR	"\033[0m"

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
	bool is_relevant;
};

struct Occurence {
	map <struct File*, int> count_hash;
};

typedef map<string, Occurence>::iterator ITR;
typedef map<struct File*, int>::iterator itr;

object_type check_type(string);

void clean_sweep();

inline char decapitalize(char);

void deserialize();

void search_for(string);

void process_query(string);

void display_index(ostream&);

void display_paths(ostream&);

void get_all_file_paths(string);

void index(string, struct File*);

void init(bool);

bool is_a_stop_word(string);

bool is_modified(struct File*);

inline bool is_not_a_word(string);

inline bool is_symbol(char c);

void mining();

void process_query(string);

void serialize();

void show_results(const char*);

inline string to_string(int);








