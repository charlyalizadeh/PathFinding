#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include "PathFindingAlg.h"





class DijkstraAlg : public PathFindingAlg
{
public:
	DijkstraAlg(int nbRow, int nbColumn, std::vector<Node*> graph, Node* start, Node* end, bool diagCount = true) : PathFindingAlg(nbRow,nbColumn,graph,start,end,diagCount)
	{}

	void algInit()
	{
		for (int i = 0;i<m_graph.size();i++)
		{
			m_graph[i]->cost = 5000;
			m_graph[i]->prev = nullptr;
			m_unvisited.push_back(m_graph[i]);
		}
		m_start->cost = 0;
	}
	bool algIteration()
	{
		m_current = *m_unvisited.begin();

		m_unvisited.erase(m_unvisited.begin());

		std::vector<Node*> neighboors;
		validNeighboors(m_current, neighboors);

		for (auto n : neighboors)
		{
			if (std::find(m_unvisited.begin(), m_unvisited.end(), n) != m_unvisited.end())
			{
				int tempDist = m_current->cost + distance(*m_current, *n);
				if (tempDist < n->cost)
				{
					n->cost = tempDist;
					n->prev = m_current;
				}
			}
		}
		std::sort(m_unvisited.begin(), m_unvisited.end(), compare);
		if (m_unvisited.empty() || (*m_unvisited.begin())->cost == 5000)
			return false;
		return true;
	}
	void alg()
	{
		algInit();
		while(algIteration())
			;
	}
	
	bool isVisited(Node* n) const
	{
		return std::find(m_unvisited.begin(), m_unvisited.end(), n) == m_unvisited.end();
	}

private:
	std::vector<Node*> m_unvisited;
};



















