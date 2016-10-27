#include <iostream>
#include <map>
#include <string>
#include <fstream>

using namespace std;

int main()
{
	ifstream file("data/bridesmaids.txt");
	string str = "";
	char c;
	while (file.get(c)) {
		cout << c << "\t" << int(c) << endl;
	}
	file.close();
	return 0;
}
