#include "stdafx.h"
#include "CAStar.h"


void CAStar::InsertNodeInWait(node * newNodo, nodeInfo * father, float value)
{
	std::list<nodeInfo*>::iterator position;
	nodeInfo* temp = new nodeInfo;
	temp->m_father = father;
	temp->m_reference = newNodo;
	temp->m_fvalue = Evaluateheuristics(&newNodo->m_position) + value;

	for (position = m_wait.begin(); position != m_wait.end(); position++) {
		if (temp->m_fvalue < (*position)->m_fvalue)
		{
			break;
		}
	}
	if (position == m_wait.end())
	{
		m_wait.push_back(temp);
	}
	else
	{
		m_wait.insert(position, temp);
	}
}

void CAStar::ClearWait()
{
	m_wait.clear();
}

bool CAStar::IsEmptyWait()
{
	return m_wait.empty();
}

nodeInfo * CAStar::NextNodeInWait()
{
	nodeInfo* temp = m_wait.front();
	m_wait.pop_front();
	return temp;
}

CAStar::CAStar()
{
}

void CAStar::InsertChilds(nodeInfo * father)
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

CAStar::~CAStar()
{
}
