#include "association.h"

#include "time.h"

int main(int argc, char *argv[])
{
	clock_t start = clock();

	string file = "params.cfg";
	Room room(file);
	room.init();

	/*furniture retrieve*/

	clock_t end = clock();
	cout << "it costs " << (end - start) / 1000 << " seconds" << endl;


	cin.get();
	return 0;
}