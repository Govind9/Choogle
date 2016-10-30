#include "filing.h"

int main ()
{
	init();
	string Query = "Who is the richest person in the world?";
	cout << "The Query Is: " << Query << endl;
	cout << "The Results Are: " << endl;
	process_query(Query);
	show_results();
	cout << "Thanks" << endl;
	return 0;
}
