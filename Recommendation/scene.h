#include "association.h"


class Scene
{
public:
	Scene(int nview);
	int determin(int index);
	bool filterMatch(pair<int, int> match);
	void getCrossView(string path);
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
	
	vector<vector<int>> count;
	vector<pair<int, int>> match;
	vector<vector<pair<int, int>>> candi_rules;
	vector<View> furniture;

};