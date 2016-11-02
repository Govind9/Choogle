#include "filing.h"

int main ()
{
	init();
	string Query;
	while (true) {
		cin >> Query;
		process_query(Query);
		cout << endl << "The Results Are:" << endl << endl;
		show_results();
		break;
	}
	
	return 0;
}
