#include "filing.h"

int main ()
{
	string begin = "./data";
	get_all_file_paths(begin);
	mining();
	deserialize();
	display_index();
	return 0;
}
