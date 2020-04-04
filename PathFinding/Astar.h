#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include "PathFindingAlg.h"


bool compareAs(const Node* lhs, const Node* rhs)
{
	return lhs->totalCost < rhs->totalCost;
}
class AStarAlg : public PathFindingAlg
{
public:
	AStarAlg(int nbRow, int nbColumn, std::vector<Node*> graph, Node* start, Node* end,bool diagCount) : PathFindingAlg(nbRow,nbColumn,graph,start,end,diagCount)
	{
		for (auto n : m_graph)
			n = (Node*)n;
	}

	void algInit()
	{ 
		m_start->totalCost = distance(*m_start, *m_end);
		m_start->cost = 0;
		m_openList.push_back(m_start);
	}
	bool algIteration()
	{
		if (!m_openList.empty())
		{
			m_current = m_openList[0];
			m_openList.erase(m_openList.begin());
			if (m_current->x == m_end->x && m_current->y == m_end->y)
				return false;

			m_closedList.push_back(m_current);

			std::vector<Node*> neighboors;
			validNeighboors(m_current, neighboors);
			for(auto n : neighboors)
			{
				int tentativeScore = m_current->cost + 1;
				if(tentativeScore<n->cost)
				{
					n->prev = m_current;
					n->cost = tentativeScore;
					n->totalCost = tentativeScore + distance(*n, *m_end);
					if (std::find(m_closedList.begin(), m_closedList.end(), n) == m_closedList.end()) 
					{			
						m_openList.push_back(n);
						std::sort(m_openList.begin(), m_openList.end(), compareAs);
					}
				}
			}
			return true;
		}
		return false;
	}
	void alg()
	{
		algInit();
		while (algIteration())
			;
	}



	bool isVisited(Node* n) const
	{
		return std::find(m_closedList.begin(), m_closedList.end(), n) != m_closedList.end();
	}

private:
	std::vector<Node*> m_openList;
	std::vector<Node*> m_closedList;
};
