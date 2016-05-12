#include "association.h"


class Scene
{
public:
	Scene(int nview);
	int determin(int index);
	bool filterMatch(pair<int, int> match);
	void getCrossView(string path);

	int findPair(int style);
	void countPair();
	void combinRule();
	void searchModel();
	void init();
	
public:   
	static string params;								  //parameters file	
	vector<pair<int, int>> cross;                 //view association
	vector<vector<int>> count;
	vector<pair<int, int>> match;
	vector<vector<pair<int, int>>> candi_rules;
	vector<View> furniture;

};