#include "scene.h"

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