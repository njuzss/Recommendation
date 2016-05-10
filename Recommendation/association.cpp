#include "association.h"
#include <algorithm>

/* bugs
*  style from 0-n-1 to 1-n done!
*  cluster number delete   done!
*/

View::View(int m, int n, string f1, string f2, string f3)
	:type(m), index(n), input_file(f1), database_file(f2), trainning_file(f3)
{
	vector<vector<Association>> L(this->nmodel);
	for (int i = 0; i < 16; i++)
	{
		cube.push_back(L);
	}
}

bool comp(pair<int, double> x, pair<int, double> y)
{
	return x.second > y.second;
}

int View::determin(int index)
{
	int num = 0;
	while ((0 < index) && (index > View::maxK))
	{
		index = index - View::maxK;
		num++;
	}

	int m = num / View::nview;
	
	return 1<<m;
}

bool View::filterRule(Association &ass)
{

	for (auto itt = ass.left.begin(); itt != ass.left.end(); itt++)
	{
		ass.model_l.insert(determin(*itt));
	}
	ass.len_l = ass.model_l.size();

	for (auto itt = ass.right.begin(); itt != ass.right.end(); itt++)
	{
		ass.model_r.insert(determin(*itt));
	}
	ass.len_r = ass.model_r.size();

	set<int> tmp(ass.model_l);
	tmp.insert(ass.model_r.begin(), ass.model_r.end());
	ass.len = tmp.size();

	if ((ass.len == 1) || ((ass.len_l + ass.len_r) != ass.len))
	{
		return false;
	}
	else
		return true;
}

void View::count()
{
	ifstream iff;
	iff.open(this->trainning_file);
	if (iff.fail())
	{
		cout << "failed to open the file: " << this->trainning_file << endl;
	}

	string line, lefty, righty, con, lif;
	string::size_type p, q, r;



	while (!iff.eof())
	{
		Association ass;
		getline(iff, line);

		/*left*/
		p = line.find_first_of(']');
		q = line.find_first_of('[');
		//support_l
		ass.sup_l = atoi(line.substr(p + 3, 3).c_str());
		//left items
		lefty = line.substr(q + 1, p - q - 1);
		r = lefty.find_first_of('=');
		ass.left.insert(atoi(lefty.substr(5, r - 5).c_str()));
		while (string::npos != (q = lefty.find_first_of(',')))
		{
			lefty = lefty.substr(q + 2);
			r = line.find_first_of('=');
			ass.left.insert(atoi(lefty.substr(5, r - 5).c_str()));
		}
		ass.len_l = ass.left.size();

		line = line.substr(p + 1);

		/*right*/
		p = line.find_first_of(']');
		q = line.find_first_of('[');
		//support_r
		ass.sup_r = atoi(line.substr(p + 3, 3).c_str());
		//right items
		righty = line.substr(q + 1, p - q - 1);
		r = righty.find_first_of('=');
		ass.right.insert(atoi(righty.substr(5, r - 5).c_str()));
		while (string::npos != (q = righty.find_first_of(',')))
		{
			righty = righty.substr(q + 2);
			r = line.find_first_of('=');
			ass.right.insert(atoi(righty.substr(5, r - 5).c_str()));
		}
		ass.len_r = ass.right.size();

		/*confidence and lift*/
		line = line.substr(p);
		p = line.find_first_of(')');
		q = line.find_first_of('(');
		con = line.substr(q + 1, p - q - 1);
		stringstream ss;
		ss << con;
		ss >> ass.conf;

		line = line.substr(p + 1);
		p = line.find_first_of(')');
		q = line.find_first_of('(');
		lif = line.substr(q + 1, p - q - 1);
		stringstream sss;
		sss << lif;
		sss >> ass.lift;

		if (this->filterRule(ass))
		{
			this->relations.push_back(ass);
		}


	}
	iff.close();
}

void View::constructCube()
{
	for (auto it = this->relations.begin(); it != this->relations.end(); it++)
	{
		int index = 0;
		for (auto itt = it->model_l.begin(); itt != it->model_l.end(); itt++)
		{
			index += (*itt);
		}

		this->cube[index][it->len_r].push_back(*it);
	}
}

void View::writeRules() const
{
	fstream ru;
	string name;
	stringstream ss(this->index);
	ss >> name;
	name = "associations_" + name + ".rule";
	ru.open(name, ios::in);
	if (!ru.fail())
	{
		ru.close();                  //if exist, return
		return;
	}
	ru.close();

	ru.open("associations.rule", ios::out);
	if (ru.fail())
	{
		cout << "cannot open rule file to write in";
	}

	for (auto it = this->relations.begin(); it != this->relations.end(); it++)
	{
		for (auto itt = (*it).left.begin(); itt != (*it).left.end(); itt++)
		{
			ru << *itt << " ";
		}
		ru << " -> ";
		for (auto itt = (*it).right.begin(); itt != (*it).right.end(); itt++)
		{
			ru << *itt << " ";
		}
		ru << endl;
	}
	ru.close();

}

void View::searchModel(int j)
{
	int index = determin(this->type);
	vector<double> result(cube[index][j].size());


	/* left */
	set<int> model_input;           //input model
	for (auto it = this->inputModel.begin(); it != this->inputModel.end(); it++)
		model_input.insert(*it);
	for (size_t k = 0; k < cube[index][j].size(); k++)
	{
		set<int> lefty(cube[index][j][k].left); //LHS of candidate rule

		vector<int> result_n(model_input.size());
		vector<int> result_v(model_input.size() + lefty.size());

		auto it_n = set_intersection(lefty.begin(), lefty.end(), model_input.begin(), model_input.end(), result_n.begin());
		result_n.resize(it_n - result_n.begin());
		if (result_n.empty())
			continue;
		auto it_v = set_union(lefty.begin(), lefty.end(), model_input.begin(), model_input.end(), result_v.begin());
		result_v.resize(it_v - result_v.begin());

		result[k] = result_n.size()*1.0 / result_v.size();


		/* confidence */
		result[k] = result[k] * cube[index][j][k].conf;
//		cout <<"rule £º"<<k <<" left * confidence : "<<result[k] << endl;

		pair<int, double> ru((int)k, result[k]);
		this->index_ru.push_back(ru);

	}
	/* top K */
	sort(this->index_ru.begin(), this->index_ru.end(), comp);
	this->index_ru.resize(K);
	/*for (int p = 0; p < K; p++)
	{
	this->candidate_rule.insert(make_pair(cube[i][j][this->index_ru[p].first], this->index_ru[p].second));
	}*/

}

void View::showRule()
{

	for (auto it = index_ru.begin(); it != index_ru.end(); it++)
	{
		cout << it->first << ". " << it->second << endl;

	}
}

void View::getCluster()
{
	string cluPath(this->trainning_file);
	cluPath = cluPath.substr(0, cluPath.size() - 4) + "clu";
	ifstream ifs(cluPath);
	if (ifs.fail())
	{
		cout << "failed to open the file: " << cluPath << endl;
	}

	int index = 0, num = 0;
	for (int x = 0; x < this->nmodel; x++)
	{
		ifs >> index >> num;
		int cluster = 0;
		vector<int> clusters;
		while (num--)
		{
			ifs >> cluster;
			clusters.push_back(cluster);
		}

		this->clu.push_back(clusters);
	}
}

void View::getDatabase()
{
	ifstream ifs(this->database_file);
	if (ifs.fail())
	{
		cout << "failed to open the file: " << this->database_file << endl;
	}
	string temp;
	int index =0, row = 0, nums = 0;
	ifs >> nums;
	while (nums--)
	{
		ifs >> index >> row;
		int style = 0;
		vector<int > model;
		for (int i = 0; i < row; i++)
		{
			ifs >> style ;
//			int tmp = style - (target - 1)*this->maxK;
			int itype = this->type - 1;
			if (find(clu[itype].begin(), clu[itype].end(), style) != clu[itype].end())
				model.push_back(style);
		}
		this->models.push_back(model);
	}

	ifs.close();
}

void View::getInput()
{
	ifstream ifs(this->input_file);
	if (ifs.fail())
	{
		cout << "failed to open the file: " << this->input_file << endl;
	}
	string temp;
	int row = 0;
	/*while (!ifs.eof())
	{
	getline(ifs,temp);
	row++;
	}*/
	ifs >> row;
	int style = 0;
	for (int i = 0; i < row; i++)
	{
		ifs >> style;
		int itype = this->type - 1;
		if (find(clu[itype].begin(), clu[itype].end(), style) != clu[itype].end())
			inputModel.push_back(style);
	}
	ifs.close();


}

void View::init(int target)
{
	this->getCluster();
	this->getInput();
	this->getDatabase();
	this->count();
	this->constructCube();
	//	this->searchModel(type,target);
	this->searchModel( 1);
}

