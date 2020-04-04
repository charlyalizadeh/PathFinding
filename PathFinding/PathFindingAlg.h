#pragma once
#include <vector>

struct Node
{
	int x, y;
	bool type;
	Node* prev;
	int cost;
	int totalCost;//Only for A*
	Node(int _x,int _y,bool _type) : x(_x),y(_y),type(_type),prev(nullptr),cost(10000),totalCost(1000)
	{}
};
bool operator<(const Node& lhs, const Node& rhs)
{
	return lhs.cost < rhs.cost;
}
bool compare(const Node* lhs, const Node* rhs)
{
	return lhs->cost < rhs->cost;
}
bool operator==(const Node& lhs, const Node& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

class PathFindingAlg
{
public :
	PathFindingAlg(int nbRow,int nbColumn,std::vector<Node*> graph,Node* start,Node* end,bool diagCount) : m_nbRow(nbRow),m_nbColumn(nbColumn),m_graph(graph),m_start(start),m_end(end),m_diagCount(diagCount)
	{}

	virtual void algInit() = 0;
	virtual bool algIteration() = 0;
	void alg()
	{
		algInit();
		while (algIteration())
			;
	}

	Node getCurrent()
	{
		return *m_current;
	}
	virtual bool isVisited(Node* n) const = 0;

protected:
	int m_nbRow, m_nbColumn;
	std::vector<Node*> m_graph;
	Node* m_start, * m_end, * m_current;
	bool m_diagCount;

	inline void validNeighboors(Node* n, std::vector<Node*>& neighboors)
	{
		for (int i = n->y - 1; i <= n->y + 1; i++)
		{
			for (int j = n->x - 1; j <= n->x + 1; j++)
			{
				if (!(i == n->y && j == n->x))
				{
					int index = i * m_nbColumn + j;
					bool diag = true;
					if (!m_diagCount)
						diag = i == n->y || j == n->x;
					if (i >= 0 && i < m_nbRow && j >= 0 && j < m_nbColumn && m_graph[index]->type && diag)
						neighboors.push_back(m_graph[index]);
				}
			}
		}
	}
	inline int distance(Node n1, Node n2)
	{
		return (n1.x - n2.x) * (n1.x - n2.x) + (n1.y - n2.y) * (n1.y - n2.y);
	}

};
