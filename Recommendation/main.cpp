#include "association.h"
#include "time.h"

string params("params.cfg");

int main(int argc, char *argv[])
{
	clock_t start = clock();

	string input, database, trainning;
	int type = 0, view = 0, target = 0;
	bool multi = false;
	ifstream ifs;
	ifs.open(params);
	if (ifs.fail())
	{
		cout << "failed to open the file: " << params << endl;
	}
	ifs >> type >> view >> input >> database >> trainning >> target;
	ifs.close();

	vector<View> furniture;
	for (int i = 1; i <= view; i++)
	{
		string tmp;
		stringstream ss;
		ss << i;
		ss >> tmp;

		string _input = input + "_" + tmp + ".view";
		string _database = database + "_" + tmp + ".txt";
		string _trainning = trainning + "_" + tmp + ".item";

		View fur(type, i, _input, _database, _trainning);
		fur.init(type, target);
		furniture.push_back(fur);
		cout << "view " << i << " done!" << endl;
		//		fur.showRule();
	}


	/*furniture retrieve*/



	clock_t end = clock();
	cout << "it costs " << (end - start) / 1000 << " seconds" << endl;



	cin.get();
	return 0;
}