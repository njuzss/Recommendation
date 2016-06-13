#include "association.h"

#include "time.h"

string Scene::params("params.cfg");
int Scene::maxK = 0;
int Scene::nmodel = 0;
int Scene::target = 0;
int Scene::type = 0;
string Scene::name("");

int main(int argc, char *argv[])
{
	clock_t start = clock();

	string input, database, trainning;

	bool multi = false;							//wait to later process

	ifstream ifs;
	ifs.open(Scene::params);
	if (ifs.fail())
	{
		cout << "failed to open the file: " << Scene::params << endl;
	}
	string tmp;
	ifs >> tmp >> Scene::type
		>> tmp >> Scene::name
		>> tmp >> input
		>> tmp >> Scene::target
		>> tmp >> database
		>> tmp >> multi
		>> tmp >> trainning
		>> tmp >> Scene::nmodel
		>> tmp >> Scene::maxK;
	ifs.close();

	string _database = database + ".data";
	Scene reco(_database, _database);
	/*get cross view*/
	string crossView = Scene::name + "_view.item";


	//	reco.init();
	//	vector<View> furniture;

	string tmp;


	string _input = input + "_" + tmp + ".view";
	string _database = database + "_" + tmp + ".data";
	string _trainning = trainning + "_" + tmp + ".item";

	Scene fur(_input, _trainning);
	fur.init();

	cout << "view " << i << " done!" << endl;
	//		fur.showRule();


	reco.init();

	/*furniture retrieve*/



	clock_t end = clock();
	cout << "it costs " << (end - start) / 1000 << " seconds" << endl;



	cin.get();
	return 0;
}