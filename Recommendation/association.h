#ifndef ASSOCIATION_H_

#define ASSOCIATION_H_


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <stdlib.h>
using namespace std;

#define ARMCHAIR (1<<0)
#define SOFA	 (1<<1)
#define ENDTABLE (1<<2)
#define TEATABLE (1<<3)

#define K 10                               //display rules

struct Association
{
	set<int> left;        //index of LHS
	set<int> right;       //index of RHS
	set<int> model_l;     //models of LHS
	set<int> model_r;     //models of RHS

	int sup_l;            //support of left
	int sup_r;            //support of right
	double conf;          //confidence
	double lift;          //lift

	size_t len_l;         //length of left
	size_t len_r;         //length of right
	size_t len;           //length of rule
};

class View
{
public:
	View(int v, string f1,  string f3);

	void getCluster();
	void getDatabase();
	void getInput();

	int determin(int index);
	bool filterRule(Association &ass);
	void count();
	void constructCube();
	void writeRules() const;

	void searchRule();
	void searchModel(int j);
	void showRule();

	void init();

//	fstream operator <<(fstream fs);

public:
	
	int index;                                          //view index
	static int type;											//model type
	static string name;
	static int maxK;                                    //max clusters
	static int nmodel;									//models in the scene
	static int nview;									//views of the model
	static int target;

	string input_file;									//input 
	vector<int> inputModel;
	//string database_file;							    //database
	//vector<vector<int>> models;
	vector<vector<int>> clu;                            //valid clusters
	string trainning_file;		                        //training
	vector<Association> relations;
	vector<vector<vector<Association>>> cube;

	vector<pair<int, double>> index_ru;                 //search result
	
	vector<Association> candi_rule;                 //result of rule


};

#endif // !ASSOCIATION_H_