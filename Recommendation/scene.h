#include "association.h"


class Scene
{
public:
	int determin(int index);
	bool filterMatch(pair<int, int> match);
	void getCrossView(string path);
	
public:   
	static string params;								  //parameters file	
	vector<pair<int, int>> cross;                 //view association
//	bool multi;
};