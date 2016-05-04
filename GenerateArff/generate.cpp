#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "stdlib.h"

//#define Trans 61
//#define Parts  480
//#define Styles 120

using namespace std;

void read(vector<vector<int>> &groups, string txtfile, int Trans)
{
	ifstream iff;
	iff.open(txtfile);
	if (iff.fail())
	{
		cout << "failed to open file!" << endl;
	}

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

		/*string::size_type next = item.find_first_of(" ");
		do
		{
		items.push_back(item.substr(0, next));
		item = item.substr(next + 1);
		next = item.find_first_of(" ");

		} while (next != string::npos);*/
		groups.push_back(items);
	}


	iff.close();
}

int main(int argc, char *argv[]){

	string path("params.cfg");
	int Trans = 0, int Parts = 0;

	ifstream ifs;
	ifs.open(path);
	if (ifs.fail())
	{
		cout << "failed to open file " << path << endl;
	}
	ifs >> Trans >> Parts;
	ifs.close();

	vector<vector<int>> groups;
	read(groups, path, Trans);

	vector<string> output(Parts, "?");
	vector<vector<string>> outputs;
	for (int i = 0; i < Trans; i++)
	{
		outputs.push_back(output);
	}

	ofstream of;
	//	string tarfile(txtfile);
	//	tarfile = tarfile.substr(0,tarfile.find_first_of('.')) + ".arff";
	//	of.open(tarfile);
	if (of.fail())
	{
		cout << "failed to generate file!";
	}

	of << "@relation 'aest'" << endl;

	for (int y = 0; y < Parts; y++)
	{
		of << "@attribute " << '\'' << "style" << y << '\'' << " " << "{ t}" << endl;
	}

	of << "@data" << endl;
	for (int x = 0; x < Trans; x++)
	{
		for (size_t y = 0; y <(groups[x].size()); y++)
			outputs[x][groups[x][y] - 1] = 't';
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
	return 0;
}