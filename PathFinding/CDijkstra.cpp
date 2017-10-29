#include "stdafx.h"
#include "CDijkstra.h"

void CDijkstra::InsertNodeInWait(node * newNodo, nodeInfo * father, float value)
{
	nodeInfo* temp = new nodeInfo;
	temp->m_father = father;
	temp->m_reference = newNodo;
	temp->m_fvalue = value;
	m_wait.push(temp);
}

void CDijkstra::ClearWait()
{
	while (!m_wait.empty())
	{
		m_wait.pop();
	}
}

bool CDijkstra::IsEmptyWait()
{
	return m_wait.empty();
}

nodeInfo * CDijkstra::NextNodeInWait()
{
	nodeInfo* temp = m_wait.front();
	m_wait.pop();

	return temp;
}

void CDijkstra::InsertChilds(nodeInfo * father)
{
	std::vector<nodeInfo*>::iterator it;
	for (it = m_reviewed.begin(); it != m_reviewed.end(); it++)
	{
		if ((*it)->m_reference == father->m_reference)
		{
			break;
		}
	}
	if (it != m_reviewed.end())
	{
		if ((*it)->m_fvalue > father->m_fvalue)
		{
			delete[](*it);
			(*it) = father;
			for (auto conection = father->m_reference->m_conections.begin(); conection != father->m_reference->m_conections.end(); conection++)
			{
				float tempValue = father->m_fvalue + conection->m_fvalue;
				InsertNodeInWait(conection->m_node, father, tempValue);
			}
		}
	}
	else
	{
		m_reviewed.push_back(father);
		for (auto conection = father->m_reference->m_conections.begin(); conection != father->m_reference->m_conections.end(); conection++)
		{
			float tempValue = father->m_fvalue + conection->m_fvalue;
			InsertNodeInWait(conection->m_node, father, tempValue);
		}
	}
}

std::vector<node*> CDijkstra::PathFinding()
{
	return PathFinding(m_end.m_reference);
}

std::vector<node*> CDijkstra::PathFinding(node *nodeStart)
{
	m_path.clear();
	nodeInfo* temp;
	std::vector<nodeInfo*>::iterator it;
	for (it = m_reviewed.begin(); it != m_reviewed.end(); it++)
	{
		if ((*it)->m_reference == nodeStart)
		{
			break;
		}
	}
	if (it == m_reviewed.end())
	{
		return m_path;
	}
	temp = (*it);
	while (temp->m_father != NULL)
	{
		m_path.push_back(temp->m_reference);
		temp = temp->m_father;
	}
	m_path.push_back(temp->m_reference);
	return m_path;
}
bool CDijkstra::EndPathFinding(nodeInfo * temp)
{
	if (temp->m_reference == m_start.m_reference)
	{
		return false;
	}

	return IsEmptyWait();
}
CDijkstra::CDijkstra()
{
}


CDijkstra::~CDijkstra()
{
}
