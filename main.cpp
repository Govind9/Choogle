#include "filing.h"

int main ()
{
	string begin = "/home/dnivog/C++/Choogle/data";
	get_all_file_paths(begin);
	mining();
	serialize();
	//display_paths();
	return 0;
}
