#include "scene.h"

Scene::Scene(int nview)
{
	vector<pair<int, int>> rules(nview);
	for (int i = 0; i < K; i++)
	{
		this->candi_rules.push_back(rules);
	}

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
	for (auto it = this->count.begin(); it != this->count.end(); it++)
	{
		for (auto itt = it->begin(); itt != it->end(); itt++)
		{
			cout << *itt << " ";
		}
		cout << endl;
	}
}

void Scene::searchModel()
{
}

void Scene::init()
{
	this->getInStyle();
	this->getPair();
	this->countPair();
	this->combinRule();
}