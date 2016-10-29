#include "filing.h"

int main ()
{
	//goto first;
	goto second;

	first:
	{
		string begin = "./data";
		get_all_file_paths(begin);
		mining();
		serialize();
	}
	return 0;

	second:
	{
		deserialize();
	}
	string Query = "Who is the richest person in the world?";
	cout << "The Query Is: " << Query << endl;
	cout << "The Results Are: " << endl;
	process_query(Query);
	show_results();
	cout << "Thanks" << endl;
	return 0;
}
