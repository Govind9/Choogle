#include <iostream>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

int main()
{
	struct stat attr;
	stat("a", &attr);
	struct stat bttr;
	stat("b", &bttr);

	cout << S_ISREG(attr.st_mode) << endl;

	cout << "a:\t" << ctime(&attr.st_mtime) << "\t" << &attr.st_mtime << endl;

	cout << "b:\t" << ctime(&bttr.st_mtime) << "\t" << &bttr.st_mtime << endl;

	return 0;
}
