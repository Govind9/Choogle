#include "filing.h"

int main (int argc, char **argv)
{
	int i;
	bool b;
	if (argc > 2) {
		if (argv[1][0] == '-' && argv[1][1] == 'i') {
			i = 2;
			b = true;
			goto choogling;
		}
		i = 1;
		b = false;
		goto choogling;
	}
	
	if (argc > 1) {
		if (argv[1][0] == '-' && argv[1][1] == 'i')
			goto indexing;
		i = 1;
		b = false;
		goto choogling;
	}

choogling:
	init(false);
	process_query(argv[i]);
	show_results();
	return 0;

indexing:
	init(true);
	return 0;
}
