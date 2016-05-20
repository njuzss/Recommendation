#include "scene.h"
#include <algorithm>
#include <numeric>

//extern bool comp(pair<int, double> x, pair<int, double> y);
bool comps(pair<int, double> x, pair<int, double> y)
{
	return x.second > y.second;
}

Scene::Scene(int nview, string file) :data_file(file)
{
	vector<pair<int, int>> rules(nview);
	for (int i = 0; i < K; i++)
	{
		this->candi_rules.push_back(rules);
	}

	this->results.resize(K);
	/*vector<int> x(K);
	for (int i = 0; i < nview; i++)
	{
	this->count.push_back(x);
	}*/
}

int Scene::determin(int index)
{
	int num = 0;
	while ((0 < index) && (index > View::maxK))
	{
		index = index - View::maxK;
		num++;
	}
	return num%View::nview;
}

bool Scene::filterMatch(pair<int, int> match)
{
	int l = determin(match.first);
	int r = determin(match.second);
	return l != r;
}

void Scene::getCrossView(string path)
{
	ifstream ifs;
	ifs.open(path);
	if (ifs.fail())
	{
		cout << "failed to open " << path << endl;
	}

	while (!ifs.eof())
	{
		string left, right;
		int x = 0, y = 0;
		string::size_type p, q;
		getline(ifs, left);

		p = left.find_first_of('=');
		x = atoi(left.substr(5, p - 5).c_str());

		right = left.substr(p + 3);

		q = right.find_first_of('=');
		y = atoi(right.substr(5, q - 5).c_str());

		pair<int, int> match(x, y);
//		cout << match.first << match.second << endl;


		if (this->filterMatch(match))
		{
			this->cross.push_back(match);
		}
	}

	ifs.close();
}

void Scene::getDatabase()
{
	//wait to split to several database - method
	ifstream ifs;
	ifs.open(this->data_file);
	if (ifs.fail())
	{
		cout << "failed to open database file" << this->data_file << endl;
	}

	Data dataset;
	string tmp;
	int num = 0;
	while (getline(ifs,tmp))
	{
		num++;
	}

	ifs.clear();                  // do not forget to clear state flag
	ifs.seekg(ios_base::beg);

	for (int i = 0; i < num; i++)
	{
		Model mo(View::nview);
		for (int j = 0; j < View::nview; j++)
		{
			int len = 0, val = 0;
			ifs >> len;
			mo[j].resize(len);
			while (val < len)
			{
				ifs >> mo[j][val++];
			}
		}		
		dataset.push_back(mo);
	}
	ifs.close(); 

	this->database.push_back(dataset);
}

void Scene::getInStyle()
{
	for (auto it = this->furniture.begin(); it != this->furniture.end(); it++)	
	{
		for (auto itt = it->candi_rule.begin(); itt != it->candi_rule.end(); itt++)
		{
			this->istyle.insert(itt->left.begin(),itt->left.end());
		}
		//this->istyle.insert(it->inputModel.begin(),it->inputModel.end());
	}
}

void Scene::getPair()
{
	for (int i = 0; i < this->cross.size(); i++)
	{
		if (istyle.count(this->cross[i].first)  && istyle.count(this->cross[i].second))
		{
			this->icross.push_back(i);
			this->vcross.insert(this->cross[i].first);
			this->vcross.insert(this->cross[i].second);
		}
	}
//	cout << this->icross.size() << endl;
}

int Scene::findPair(int style)
{
	int count = 0;
	for (auto it = this->icross.begin(); it != this->icross.end(); it++)
	{
		if (this->cross[*it].first == style || this->cross[*it].second == style)
		{
			count++;
		}
	}
	return count;
}

void Scene::countPair()
{
	for (int i = 0; i < this->furniture.size(); i++)
	{
		vector<int> result;
		for (auto it = this->furniture[i].candi_rule.begin(); it != this->furniture[i].candi_rule.end(); it++)
		{
			int pairs = 0;
			set<int> left(it->left);	
			for (auto itt = left.begin(); itt != left.end(); itt++)
			{
				pairs += findPair(*itt);
			}
			result.push_back(pairs);
		}
		this->count.push_back(result);
	}
}

void Scene::combinRule()
{

	

	/*for (auto it = this->count.begin(); it != this->count.end(); it++)
	{
	for (auto itt = it->begin(); itt != it->end(); itt++)
	{
	cout << *itt << " ";
	}
	cout << endl;
	}*/
}

void Scene::searchModel()
{
	for (int i = 0; i < K; i++)
	{
		for (int k = 0; k < this->database[0].size(); k++)
		{
			vector<double> v_result(View::nview);
			for (int j = 0; j < View::nview; j++)
			{
				set<int> righty(this->furniture[j].candi_rule[i].right);
				vector<int> result_n(this->database[0][k][j].size());
				vector<int> result_v(this->database[0][k][j].size() + righty.size());

				auto it_n = set_intersection(righty.begin(), righty.end(), this->database[0][k][j].begin(), this->database[0][k][j].end(), result_n.begin());
				result_n.resize(it_n - result_n.begin());
				if (result_n.empty())
					continue;

//				cout << "fuck you" << endl;
				auto it_v = set_union(righty.begin(), righty.end(), this->database[0][k][j].begin(), this->database[0][k][j].end(), result_v.begin());
				result_v.resize(it_v - result_v.begin());

				v_result[j] = result_n.size()*1.0 / result_v.size();
				v_result[j] = v_result[j] * this->furniture[j].index_ru[i].second;
			}

			double m_result = accumulate(v_result.begin(), v_result.end(), 0.0);
			pair<int, double> tmp(k,m_result);
			this->results[i].push_back(tmp);
		}
	}
	for (int i = 0; i < K; i++)
	{
		sort(this->results[i].begin(), this->results[i].end(), comps);
	}
	
}

void Scene::init()
{
	this->getDatabase();
	this->searchModel();
	//this->getInStyle();
	//this->getPair();
	//this->countPair();
	//this->combinRule();
}