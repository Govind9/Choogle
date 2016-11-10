#include "filing.h"

int main (int argc, char **argv)
{
	init(true);

	process_query(argv[1]);
	show_results();
	
	return 0;
}
