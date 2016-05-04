#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
using namespace std;

#define ARMCHAIR (1<<0)
#define SOFA	 (1<<1)
#define ENDTABLE (1<<2)
#define TEATABLE (1<<3)

#define FURNI 4

#define Trans 64

#define Parts  240

#define K 10

struct Association
{
	set<int> left;     //index of LHS
	set<int> right;    //index of RHS
	set<int> model_l;  //models of LHS
	set<int> model_r;  //models of RHS

	int sup_l;         //support of left
	int sup_r;         //support of right
	double conf;       //confidence
	double lift;       //liftness

	int len_l;         //length of left
	int len_r;         //length of right
	int len;           //length of rule
};

class View
{
public:
	View(int m, int n, string f1, string f2, string f3);

	//	static bool comp(pair<int, double> x, pair<int, double> y);

	int determin(int index);

	bool filterRule(Association &ass);

	void getCluster();
	void getDatabase(int target);
	void getInput(int type);

	void count();

	void constructCube();

	void writeRules() const;

	void searchModel(int i, int j);
	void showRule();

	void init(int type, int target);

	static void getCrossView();

	//	fstream operator <<(fstream fs);

public:
	int type;											//model type
	int index;                                          //view index

	string input_file;									//input 
	vector<int> inputModel;
	string database_file;							    //database
	vector<vector<int>> models;
	vector<vector<int>> clu;                            //valid clusters

	string trainning_file;		                        //trainning
	vector<Association> relations;
	vector<vector<vector<Association>>> cube;

	vector<pair<int, double>> index_ru;
	//	map<Association,double> candidate_rule;                 //result of rule
	//	vector<pair<int, double>> candi_mo;					//reuslt of model

};