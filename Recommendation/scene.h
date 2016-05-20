#include "association.h"

typedef vector<vector<int>> Model;
typedef vector<Model> Data;

//struct Model
//{
//	vector<vector<int>> mView;
//};

class Scene
{
public:
	Scene(int nview, string file);
	int determin(int index);
	bool filterMatch(pair<int, int> match);
	void getCrossView(string path);

	void getDatabase();

	void getInStyle();
	void getPair();

	int findPair(int style);
	void countPair();
	void combinRule();
	void searchModel();
	void init();
	
public:   
	static string params;					      //parameters file	
	set<int> istyle;							  //input models styles
	vector<pair<int, int>> cross;                 //view association
	vector<int> icross;                           //index of related cross 
	set<int> vcross;

	string data_file;                             //file name of database
	vector<Data> database;						  //database

	vector<vector<pair<int,double>>> results;			      //search results

	vector<vector<int>> count;
	vector<pair<int, int>> match;
	vector<vector<pair<int, int>>> candi_rules;
	vector<View> furniture;

};