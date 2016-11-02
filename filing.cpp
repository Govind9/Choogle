#include "filing.h"

vector <struct File> Files;

long int previous_index_time = 0; //should be set at the time of indexing and stored in some file for retrieval

long int pmt1, pmt2;

map <string, Occurence> word_hash;

string the_dir;

void init()
{
	long int modified_at;
	int fd;
	the_dir = "/home/";
	the_dir += getenv("USER");
	the_dir += "/.Choogle";
	struct stat s;
	ifstream file;
	string path;
	ofstream obj;

	if (stat(the_dir.c_str(), &s) != 0)
		goto rebuild_dir;
	
	if (stat((the_dir + THE_FILES_FILE).c_str(), &s) != 0)
		goto rebuild_files;
	modified_at = s.st_mtime;
	
	if (stat((the_dir + THE_INDEX_FILE).c_str(), &s) != 0)
		goto rebuild_files;

	//Read Previous modification times for both files:
	file.open((the_dir + THE_FILES_FILE).c_str());
	file >> pmt1;
	file.close();
	if (modified_at - pmt1 > ONE_MINUTE)
		goto rebuild_files;
	
	modified_at = s.st_mtime;
	file.open((the_dir + THE_INDEX_FILE).c_str());
	file >> pmt2;
	file.close();
	if (modified_at - pmt2 > ONE_MINUTE)
		goto rebuild_files;
	
	//Everything set.
	deserialize();
	return;

rebuild_dir:
	cout << "Making Directory ..." << endl;
	mkdir(the_dir.c_str(), 0777);
	cout << "Directory Made." << endl;
rebuild_files:
	cout << "Making files ..." << endl;
	path = the_dir;
	path += THE_FILES_FILE;
	fd = open(path.c_str(), O_RDWR | O_CREAT, 0777);
	if (fd == -1)
		cout << "Linux is being a bitch." << endl;
	close(fd);

	path = the_dir;
	path += THE_INDEX_FILE;
	fd = open(path.c_str(), O_RDWR | O_CREAT, 0777);
	if (fd == -1)
		cout << "Linux is being a bitch." << endl;
	close(fd);
	cout << "Files Made." << endl;

rebuild_index:
	path = "/home/";
	path += getenv("USER");
	path += "/data";
	get_all_file_paths(path);
	mining();
	serialize();
}

void search_for(string keyword)
{
	if (word_hash.count(keyword) != 1)
		return;
	
	struct Occurence occ = word_hash[keyword];

	for (itr i = occ.count_hash.begin(); i != occ.count_hash.end(); ++i)
		i->first->is_relevant = true;
}

void show_results()
{
	for (int i = 0, j = 1; i < Files.size(); i++) {
		if (Files[i].is_relevant) {
			cout << j++ << "\t" << Files[i].name << "\t" << Files[i].path << endl << endl;
		}
	}
}

void process_query(string query)
{
	string str = "";
	char c;

	//Clean out the relevancy of the previous queries.
	for (int i = 0; i < Files.size(); i++)
		Files[i].is_relevant = false;

	for (int i = 0; i < query.length(); i++) {
		c = query[i];

		if (is_symbol(c)) {
			if (is_not_a_word(str)) {
				str = "";
				continue;
			}
			if (is_a_stop_word(str)) {
				str = "";
				continue;
			}

			//We have a genuine keyword.
			search_for(str);
			str = "";
			continue;
		}

		c = decapitalize(c);
		str += c;
	}
}

inline void clean_sweep()
{
	remove((the_dir + THE_INDEX_FILE).c_str());
	remove((the_dir + THE_FILES_FILE).c_str());
}

//Implement exception handling, in case the files are missing.
//Also hadnle when data is not according to expectation i.e. handle possible file tampering.
void deserialize()
{
	char c;
	int n = 0;
	string str;
	ifstream file((the_dir + THE_FILES_FILE).c_str());
	file >> pmt1;
	file >> previous_index_time;
	file >> n;
	file.get(c);
	for (int i = 0; i < n; i++) {
		struct File obj;
		file >> obj.id;
		file.get(c);
		
		str = "";
		while (true) {
			file.get(c);
			if (c == '\t')
				break;
			str += c;
		}
		obj.name = str;

		str = "";
		while (true) {
			file.get(c);
			if (c == '\n')
				break;
			str += c;
		}
		obj.path = str;
		Files.push_back(obj);
	}
	file.close();

	file.open((the_dir + THE_INDEX_FILE).c_str());
	file >> pmt2;
	file.get(c);
	while (file.get(c)) {
		struct Occurence occ;
		string word = "";
		int id;
		int freq;
		word = "";
		while (c != '\t') {
			word += c;
			file.get(c);
		}

		id = 0;
		freq = 0;
		while (c != '\n') {
			file >> id;
			file >> freq;
			occ.count_hash[&Files[id]] = freq;
			file.get(c);
		}
		word_hash[word] = occ;
	}
	file.close();
}

//Maybe simply call both of the display functions and redirect their output to the files.
void serialize()
{
	clean_sweep();
	ofstream file((the_dir + THE_FILES_FILE).c_str());
	file << time(0) << "\t";
	file << previous_index_time << "\t" << Files.size() << "\n";
	for (int i = 0; i < Files.size(); i++) {
		file << Files[i].id << "\t";
		file << Files[i].name << "\t";
		file << Files[i].path << "\n";
	}
	file.close();

	file.open((the_dir + THE_INDEX_FILE).c_str());
	file << time(0) << "\t";
	for (ITR i = word_hash.begin(); i != word_hash.end(); ++i) {
		file << i->first << "\t";
		for (itr j = i->second.count_hash.begin(); j != i->second.count_hash.end(); ++j) {
			if (j != i->second.count_hash.begin())
				file << ";";
			file << j->first->id << " " << j->second;
		}
		file << "\n";
	}
	file.close();
}

inline bool is_not_a_word(string word)
{
	return (word.length() < 2);
}

bool is_a_stop_word(string word)
{
	ifstream file("stopword.txt");
	char c;
	string str = "";
	while (file.get(c)) {
		if (c == ' ') {
			if (str == word)
				return true;
			str = "";
			continue;
		}
		str += c;
	}
	file.close();
	return false;
}

inline string to_string(int i)
{
	stringstream convert;
	convert << i;
	return convert.str();
}

void display_index()
{
	for (ITR i = word_hash.begin(); i != word_hash.end(); ++i) {
		cout << i->first << " : ";
		for (itr j = i->second.count_hash.begin(); j != i->second.count_hash.end(); ++j) {
			cout << j->first->id << " " << j->second << "; ";
		}
		cout << endl;
	}
}

void index(string word, struct File *file)
{
	if (is_not_a_word(word))
		return;
	if (is_a_stop_word(word))
		return;
	
	if (word_hash.count(word) == 1) {
		if (word_hash[word].count_hash.count(file) == 1) {
			word_hash[word].count_hash[file]++;
			return;
		}
		word_hash[word].count_hash[file] = 1;
		return;
	}
	
	Occurence occ;
	occ.count_hash[file] = 1;
	word_hash[word] = occ;
}

inline char decapitalize(char c)
{
	return (c >= 'A' && c <= 'Z')? c - 'A' + 'a': c;
}

inline bool is_symbol(char c)
{
	return ((c <= '/') || (c >= ':' && c <= '@') 
			|| (c >= '{' && c <= '~') || (c >= '[' && c <= '`'));
}

//Add exception handling in case some files do not open.
void mining()
{
	ifstream file;
	char c;
	string str;
	
	for (int i = 0; i < Files.size(); i++) {
		if (Files[i].is_modified)
			//continue;
		/*
		  There are some deep issues here hence I am abandoning this feature for now.
		  Maybe later I will comeback and make this meaning full.
		*/
		
		//Mining the file name.
		str = "";
		for (int j = 0; j < Files[i].name.length(); j++) {
			c = Files[i].name[j];
			if (is_symbol(c)) {
				index(str, &Files[i]);
				str = "";
				continue;
			}
			c = decapitalize(c);
			str += c;
		}
		//Mining the contents of the file.
		file.open(Files[i].path.c_str());
		str = "";
		while (file.get(c)) {
			if (is_symbol(c)) {
				index(str, &Files[i]);
				str = "";
				continue;
			}
			c = decapitalize(c);
			str += c;
		}
		file.close();
	}
	previous_index_time = time(0);
}

bool is_modified(struct File *file)
{
	struct stat obj;
	stat(file->path.c_str(), &obj);
	long int modified_at = obj.st_mtime;

	return (modified_at > previous_index_time);
}

void display_paths()
{
	cout << Files.size() << endl;
	for (int i = 0; i < Files.size(); i++) {
		cout << Files[i].path << endl;
	}
}

object_type check_type(string path)
{
	int i = path.length() - 1;
	struct stat path_stat;
	char c = '.';

	while ( c != '/') {
		c = path[i];
		i--;
	}
	if (path[i + 2] == '.')
		return Hidden;

	stat(path.c_str(), &path_stat);

	if (S_ISLNK(path_stat.st_mode) == 1)
		return Lnk;

	if (S_ISDIR(path_stat.st_mode) == 1)
		return Dir;

	if (S_ISREG(path_stat.st_mode) == 1)
		return File;
	
	return Other;
}


void get_all_file_paths(string begin)
{
	DIR *root;
	struct dirent *temp;
	int i = -1;

	//Get the last stored indexing time.
	ifstream f(THE_FILES_FILE);
	f >> previous_index_time;
	f.close();

	root = opendir(begin.c_str());

	while ((temp = readdir(root))) {
		string path = begin + "/" + temp->d_name;
		if (check_type(path) == File) {
			struct File file;
			file.id = ++i;
			file.name = temp->d_name;
			file.path = path;
			file.is_modified = is_modified(&file);
			file.is_relevant = false;
			Files.push_back(file);
			continue;
		}
		if (check_type(path) == Dir)
			get_all_file_paths(path);
	} 
	closedir(root);
}
