#include "filing.h"

vector <struct File> Files;

long int previous_index_time = 0; //should be set at the time of indexing and stored in some file for retrieval

map <string, Occurence> word_hash;

inline void clean_sweep()
{
	remove(THE_INDEX_FILE);
	remove(THE_FILES_FILE);
}

void deserialize()
{
	char c;
	int n = 0;
	string str;
	ifstream file(THE_FILES_FILE);
	file >> previous_index_time;
	file >> n;
	file.get(c);
	for (int i = 0; i < n; i++) {
		struct File obj;
		file >> obj.id;
		file.get(c);
		cout << obj.id << endl;
		
		while (true) {
			file.get(c);
			if (c == '\t')
				break;
			str += c;
		}
		obj.name = str;

		while (true) {
			file.get(c);
			if (c == '\n')
				break;
			str += c;
		}
		file >> obj.path;
		Files.push_back(obj);
	}
	file.close();

	file.open(THE_INDEX_FILE);
	struct Occurence occ;
	string word;
	int id;
	int freq;
	while (!file.eof()) {
		file >> word;
		file.get(c);
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

void serialize()
{
	clean_sweep();
	ofstream file(THE_FILES_FILE);
	file << previous_index_time << "\t" << Files.size() << "\n";
	for (int i = 0; i < Files.size(); i++) {
		file << Files[i].id << "\t";
		file << Files[i].name << "\t";
		file << Files[i].path << "\n";
	}
	file.close();

	file.open(THE_INDEX_FILE);
	typedef map<string, Occurence>::iterator ITR;
	typedef map<struct File*, int>::iterator itr;

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
	if (word.length() < 2)
		return true;
	return false;
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
	typedef map<string, Occurence>::iterator ITR;
	typedef map<struct File*, int>::iterator itr;

	for (ITR i = word_hash.begin(); i != word_hash.end(); ++i) {
		cout << i->first << " : ";
		for (itr j = i->second.count_hash.begin(); j != i->second.count_hash.end(); ++j) {
			cout << j->first->name << " " << j->second << "; ";
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

inline bool is_symbol(char c)
{
	if ((c <= '/') || (c >= ':' && c <= '@') 
			|| (c >= '{' && c <= '~') || (c >= '[' && c <= '`'))
		return true;
	return false;
}

void mining()
{
	ifstream file;
	char c;
	string str;

	for (int i = 0; i < Files.size(); i++) {
		if (!Files[i].is_modified)
			continue;
		
		file.open(Files[i].path.c_str());
		str = "";
		while (file.get(c)) {
			if (is_symbol(c)) {
				index(str, &Files[i]);
				str = "";
				continue;
			}
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

	if (modified_at > previous_index_time)
		return true;
	return false;
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

	root = opendir(begin.c_str());

	while ((temp = readdir(root))) {
		string name = begin + "/" + temp->d_name;
		if (check_type(name) == File) {
			struct File file;
			file.id = ++i;
			file.name = temp->d_name;
			file.path = name;
			file.is_modified = is_modified(&file);
			Files.push_back(file);
			continue;
		}
		if (check_type(name) == Dir)
			get_all_file_paths(name);
	} 
	closedir(root);
}
