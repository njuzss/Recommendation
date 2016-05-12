#include "association.h"
#include "scene.h"
#include "time.h"

string Scene::params("params.cfg");
int View::maxK = 0;
int View::nmodel = 0;
int View::nview = 0;
int View::target = 0;
int View::type = 0;
string View::name("");

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
	ifs >> View::type >> View::name>> input >> View::target >> database >> multi >> trainning >> View::nmodel >> View::nview >> View::maxK;
	ifs.close();

	Scene reco(View::nview);
	/*get cross view*/
	string crossView = View::name + "_view.item";
	reco.getCrossView(crossView);

//	vector<View> furniture;
	for (int i = 1; i <= View::nview; i++)
	{
		string tmp;
		stringstream ss;
		ss << i;
		ss >> tmp;

		string _input = input + "_" + tmp + ".view";
		string _database = database + "_" + tmp + ".data";
		string _trainning = trainning + "_" + tmp + ".item";

		View fur( i, _input, _database, _trainning);
		fur.init();
		reco.furniture.push_back(fur);
		cout << "view " << i << " done!" << endl;
//		fur.showRule();
	}

	reco.init();

	/*furniture retrieve*/



	clock_t end = clock();
	cout << "it costs " << (end - start) / 1000 << " seconds" << endl;



	cin.get();
	return 0;
}