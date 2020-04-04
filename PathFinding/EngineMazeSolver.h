#pragma once
#include "olcConsoleGameEngine.h"
#include "DijkstraAlg.h"
#include "Astar.h"
#include "Maze.h"
#include <string>

enum ALG
{
	DIJKSTRA,
	ASTAR,
};

class EngineMazeSolver : public olcConsoleGameEngine
{

public:
	EngineMazeSolver(int nbRow, int nbColumn,bool displayStep, ALG algType,bool isMaze = false,int seed = 000000) : m_state(0),m_displayStep(displayStep),m_alg(nullptr), m_algType(algType), m_isMaze(isMaze),m_seed(seed)
	{
		if (nbRow % 2 == 0)
			nbRow++;
		if (nbColumn % 2 == 0)
			nbColumn++;
		if (m_isMaze)
		{
			m_maze = new Maze((nbRow / 2), (nbColumn / 2), seed);
			while (m_maze->backTrackingIteration())
				;
			std::vector<bool> buffer = m_maze->convertToBooleanMap(1);
			for (int i = 0; i < nbRow; i++)
			{
				for (int j = 0; j < nbColumn; j++)
				{
					m_graph.push_back(new Node{ j,i,!buffer[i * nbColumn + j] });
				}
			}
		}
		else
		{
			for (int i = 0; i < nbRow; i++)
			{
				for (int j = 0; j < nbColumn; j++)
				{
					m_graph.push_back(new Node{ j,i,true });
				}
			}
		}
		ConstructConsole(nbColumn, nbRow, 12,12);
		m_start = new Node{ -1,-1,true };
		m_end = new Node{ -1,-1,true };
	}
private:
	PathFindingAlg* m_alg;
	Maze* m_maze;
	Node* m_start, *m_end;
	std::vector<Node*> m_graph;
	int m_state;
	bool m_displayStep, m_isMaze;
	ALG m_algType;
	int m_seed;
	



	bool placeStart()
	{
		if (GetKey(VK_LBUTTON).bPressed && m_graph[GetMouseY() * ScreenWidth() + GetMouseX()]->type)
		{
			m_start->x = GetMouseX();
			m_start->y = GetMouseY();
		}
		if (GetKey(VK_RETURN).bPressed && validCoord(m_start->x, m_start->y))
		{
			m_graph[m_start->y * ScreenWidth() + m_start->x] = m_start;
			return true;
		}
		return false;
	}
	bool placeEnd()
	{
		if (GetKey(VK_LBUTTON).bPressed && m_graph[GetMouseY() * ScreenWidth() + GetMouseX()]->type)
		{
			m_end->x = GetMouseX();
			m_end->y = GetMouseY();
		}
		if (GetKey(VK_RETURN).bPressed && validCoord(m_end->x, m_end->y))
		{
			m_graph[m_end->y * ScreenWidth() + m_end->x] = m_end;
			return true;
		}
		return false;
	}
	bool placeWall()
	{
		if (GetKey(VK_LBUTTON).bPressed || GetKey(VK_LBUTTON).bHeld)
		{
			m_graph[GetMouseY() * ScreenWidth() + GetMouseX()]->type = false;
		}
		if (GetKey(VK_RBUTTON).bPressed || GetKey(VK_RBUTTON).bHeld)
		{
			m_graph[GetMouseY() * ScreenWidth() + GetMouseX()]->type = true;
		}
		if (GetKey(VK_RETURN).bPressed)
			return true;
		return false;
	}
	inline bool validCoord(int x, int y)
	{
		return x >= 0 && x < ScreenWidth() && y >= 0 && y < ScreenHeight();
	}

	void displayMap()
	{
		for(auto n : m_graph)
		{
			if (m_alg!=nullptr && m_displayStep && m_alg->isVisited(n))
				Draw(n->x, n->y, 9608, FG_DARK_GREEN);
			else if (n->type)
				Draw(n->x, n->y, 9608, FG_GREY);
			else
				Draw(n->x, n->y, 9608, FG_BLACK);
		}
		if (validCoord(m_start->x, m_start->y))
			Draw(m_start->x, m_start->y, 9608, FG_CYAN);
		if (validCoord(m_end->x, m_end->y))
			Draw(m_end->x, m_end->y, 9608, FG_MAGENTA);
		
	}
	void displayPath()
	{
		int i = 0;
		Node* buffer = m_end;
		while (buffer != nullptr)
		{
			Draw(buffer->x, buffer->y, 9608, FG_RED);
			buffer = buffer->prev;
			i++;
		}
		DrawString(0, 0, std::to_wstring(i), BG_DARK_RED | FG_WHITE);
	}
	void displayCurrent()
	{
		Node current = m_alg->getCurrent();
		Draw(current.x, current.y, 9608, FG_GREEN);
	}



protected:
	bool OnUserCreate()
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), 9608, FG_BLACK);
		displayMap();
		switch (m_state)
		{
		case 0:
			if (placeStart())
				m_state++;
			break;
		case 1:
			if (placeEnd())
			{
				if (m_isMaze)
				{
					m_state = 3;
					if (m_algType == ASTAR)
						m_alg = new AStarAlg(ScreenHeight(), ScreenWidth(), m_graph, m_start, m_end, false);
					else
						m_alg = new DijkstraAlg(ScreenWidth(), ScreenHeight(), m_graph, m_start, m_end, false);
					if (m_displayStep)
						m_alg->algInit();
					else
					{
						m_alg->alg();
						m_state = 4;
					}
				}
				else
					m_state = 2;
			}
			break;
		case 2:
			if (placeWall())
			{
				m_state++;
				if (m_algType == ASTAR)
					m_alg = new AStarAlg(ScreenHeight(), ScreenWidth(), m_graph, m_start, m_end, false);
				else
					m_alg = new DijkstraAlg(ScreenWidth(), ScreenHeight(), m_graph, m_start, m_end, false);
				m_alg->algInit();
			}
			break;
		case 3:
			if (!m_alg->algIteration())
				m_state++;
			break;
		case 4:
			displayPath();
			break;
		}
		return true;
	}
};