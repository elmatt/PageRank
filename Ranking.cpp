#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>

using namespace std;
struct Edge{
	vector<int> dest;
};

vector<int> node_connections()
{
	string line;
	ifstream infile("pagerankings.txt");
	vector<int> v;
	while (getline(infile, line))
	{
		stringstream iss(line);
		int n;
		while (iss >>n)
		{
			v.push_back(n);
		}
	}
	infile.close();
	return v;
}

void output(map<int, float> mymap)
{
	char letter ='A';
	for (map<int,float>::const_iterator it =mymap.begin(); it!=mymap.end(); ++it)
	{
		cout << "Node " << letter << " - " <<fixed << setprecision(2) <<it->second << "%" <<endl;
		letter = char(letter+1);
	}
}

int main()
{
	vector<int> v = node_connections();
	vector<int> nodes;
	int prev;
	double prob;
	double lam = 0.82;
	map<int, float> init_probability;
	map<int, float> actual_probability;

	//initialize node list
	for (int i=0; i < v.size(); i++)
	{
		if (find(nodes.begin(), nodes.end(), v[i]) != nodes.end())
		{
			//found
		}
		else //not found
		{
			nodes.push_back(v[i]);
		}

	}

	//connect everything in graph
	Edge *graph = new Edge[nodes.size()];
	for (int i=1; i < v.size(); i+=2)
	{
		prev = v[i-1];
		graph[prev].dest.push_back(v[i]);
	}


	//initialize probability of all nodes to (1/node_size) * (1/out_going_links)
	for (int i=0; i < nodes.size(); i++)
	{
		if (graph[i].dest.size() !=0)
			init_probability.insert(pair<int,float>(i, 1.0/nodes.size()*1.0/graph[i].dest.size()));
		else
			init_probability.insert(pair<int,float>(i, 1.0/nodes.size()));

	}

	float sum_prob =0;
	float new_prob=0;
	for (int i=0; i < nodes.size(); i++) //looking at current node
	{
		for (int k =0; k < nodes.size(); k++) //for every other node
		{
			if (k != i)
			{
				for (int j=0; j < graph[k].dest.size(); j++) //find what links back to original node
				{
					if (i == graph[k].dest[j]) //sum the probabilities
						sum_prob+= init_probability[k];
				}
			}
		}
		new_prob = 0.18*(1.0/nodes.size()) + (0.82)*sum_prob;
		actual_probability.insert(pair<int,float>(i, 100*new_prob));
		sum_prob =0.0;
	}

	output(actual_probability);
}
