#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
int Trans = 0;
int Types = 0;
vector<int> nums;
vector<vector<int>> groups;
string scene;

vector<vector<string>> outputs;


int maxIndex()
{
	vector<int> each(Trans);
	for (int i = 0; i < Trans; i++)
	{
		each[i] = groups[i][groups[i].size() - 1];
	}

	return *max_element(each.begin(), each.end());
}

void read()
{
	ifstream iff;
	iff.open(scene);
	if (iff.fail())
	{
		cout << "failed to open file!"<< scene << endl;
	}
	groups.clear();

	for (int i = 0; i < Trans; i++)
	{
		string item;
		vector<int> items;
		getline(iff, item);

		istringstream iss(item);
		int a;
		while (iss >> a){
			items.push_back(a);
		}
		groups.push_back(items);
	}


	iff.close();
}

void write()
{
	size_t Parts = maxIndex();
	cout << Parts;
	vector<string> output(Parts, "?");
	outputs.clear();
	for (int i = 0; i < Trans; i++)
	{
		outputs.push_back(output);
	}

	ofstream of;
	string tarfile(scene);
	tarfile = tarfile.substr(0,tarfile.find_first_of('.')) + ".arff";
	of.open(tarfile);
	if (of.fail())
	{
		cout << "failed to generate file!" << tarfile << endl;
	}

	of << "@relation 'aest'" << endl;

	for (int y = 1; y <= Parts; y++)
	{
		of << "@attribute " << '\'' << "style" << y << '\'' << " " << "{ t}" << endl;
	}

	of << "@data" << endl;
	for (int x = 0; x < Trans; x++)
	{
		for (size_t y = 0; y <(groups[x].size()); y++)
			outputs[x][groups[x][y] - 1] = "t";
	}

	for (int i = 0; i < Trans; i++)
	{
		of << outputs[i][0];
		for (int j = 1; j < Parts; j++)
		{
			of << "," << outputs[i][j];
		}
		of << "\n";
	}

	of.close();
}

int main(int argc, char *argv[]){

	string path("params.cfg");

	ifstream ifs;
	ifs.open(path);
	if (ifs.fail())
	{
		cout << "failed to open file " << path << endl;
	}
	ifs >> scene >> Trans >> Types;
	nums.resize(Types);
	for (int i = 0; i < Types; i++)
	{
		ifs >> nums[i];
	}

	ifs.close();

	groups.resize(Types);
	
	read();
	write();
	return 0;
}