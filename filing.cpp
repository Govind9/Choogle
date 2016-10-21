#include "filing.h"

vector <struct File> Files;

long int previous_index_time = 0; //should be set at the time of indexing and stored in some file for retrieval

void index(string word)
{
	cout << word << endl;
}

inline bool is_symbol(char c)
{
	if ( (c >= 0 && c <= '/') || (c >= ':' && c <= '@') 
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
				index(str);
				str = "";
				continue;
			}
			str += c;
		}
		file.close();
	}
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

	root = opendir(begin.c_str());

	while ((temp = readdir(root))) {
		string name = begin + "/" + temp->d_name;
		if (check_type(name) == File) {
			struct File file;
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
