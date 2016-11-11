#include "filing.h"

int main (int argc, char **argv)
{
	if (argc > 2) {
		cout << "1\n";
		if (argv[1][0] == '-' && argv[1][1] == 'i') {
			cout << "2\n";
			init(true);
			process_query(argv[2]);
			show_results();
			return 0;
		}
		cout << "3\n";
		init(false);
		process_query(argv[1]);
		show_results();
		return 0;
	}
	
	if (argc > 1) {
		cout << "4\n";
		cout << argv[1] << endl;
		if (argv[1][0] == '-' && argv[1][1] == 'i') {
			cout << "5\n";
			init(true);
			return 0;
		}
		cout << "6\n";
		init(false);
		process_query(argv[1]);
		show_results();
		return 0;
	}
	return 0;
}
