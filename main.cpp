#include "filing.h"

int main (int argc, char **argv)
{
	string arg;
	bool b = false;;
	string opener = GENEREL_OPENER;
	for (int i = 1; i < argc; i++) {
		arg = string(argv[i]);
		if (arg == "-i" || arg == "--index") {
			b = true;
			continue;
		}
		if (arg == "-t" || arg == "--terminal") {
			opener = TERMINAL_OPENER;
			continue;
		}
		if (arg == "-ti" || arg == "-it") {
			opener = TERMINAL_OPENER;
			b = true;
			continue;
		}
		goto choogling;
	}
	init(b);
	return 0;
 
choogling:
	init(b);
	process_query(arg);
	show_results(arg, opener);
	return 0;
}
