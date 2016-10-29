#include "filing.h"

int main ()
{
	string begin = "./data";
	get_all_file_paths(begin);
	mining();
	serialize();
	return 0;
}
