#include "stdafx.h"
#include "CGraph.h"

void CGraph::CreateGraph(int rows, int columns, float distanceX, float distanceY)
{
	m_graph.clear();
	m_graph.resize(rows*columns);
	int counter = 0;
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			int index = column + row*columns;
			m_graph[index].m_id = counter;
			m_graph[index].m_position.x = 240 + distanceX + 2 * distanceX*column;
			m_graph[index].m_position.y = distanceY + 2*distanceY*row;
			if (row != 0)
			{
				if (column != 0)
				{
					m_graph[index].m_conections.push_back((*new Conection(0, &m_graph[index - columns - 1])));
				}
				m_graph[index].m_conections.push_back((*new Conection(0, &m_graph[index - columns])));
				if (column != columns - 1)
				{
					m_graph[index].m_conections.push_back((*new Conection(0, &m_graph[index - columns + 1])));
				}
			}
			if (row != rows - 1)
			{
				if (column != 0)
				{
					m_graph[index].m_conections.push_back((*new Conection(0, &m_graph[index + columns - 1])));
				}
				m_graph[index].m_conections.push_back((*new Conection(0, &m_graph[index + columns])));
				if (column != columns - 1)
				{
					m_graph[index].m_conections.push_back((*new Conection(0, &m_graph[index + columns + 1])));
				}
			}
			if (column != 0)
			{
				m_graph[index].m_conections.push_back((*new Conection(0, &m_graph[index - 1])));
			}
			if (column != columns - 1)
			{
				m_graph[index].m_conections.push_back((*new Conection(0, &m_graph[index + 1])));
			}
			counter++;
		}
	}
}

void CGraph::SetStart(node * start)
{
	m_start = start;
}

void CGraph::SetEnd(node * end)
{
	m_end = end;
}

void CGraph::DisconectNode(node * node)
{
	std::vector<Conection>::iterator deleteConection;
	
	for (auto connection = node->m_conections.begin(); connection != node->m_conections.end(); connection++)
	{
		for (auto conNode = connection->m_node->m_conections.begin(); conNode != connection->m_node->m_conections.end(); conNode++)
		{
			if ((*conNode).m_node == node)
			{
				deleteConection = conNode;
			}
		}
		connection->m_node->m_conections.erase(deleteConection);
	}
	node->m_conections.clear();
}

void CGraph::ChangeValueConnections(node * node, float value)
{
	std::vector<Conection>::iterator conectionForNode;

	for (auto connection = node->m_conections.begin(); connection != node->m_conections.end(); connection++)
	{
		for (auto conNode = connection->m_node->m_conections.begin(); conNode != connection->m_node->m_conections.end(); conNode++)
		{
			if ((*conNode).m_node == node)
			{
				conectionForNode = conNode;
				break;
			}
		}
		conectionForNode->m_fvalue = value;
	}
	node->m_conectionValue = value;
}

CGraph::CGraph()
{
	m_start = NULL;
	m_end = NULL;
}


CGraph::~CGraph()
{
}
