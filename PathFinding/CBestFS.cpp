#include "stdafx.h"
#include "CBestFS.h"

void CBestFS::InsertNodeInWait(node * newNodo, nodeInfo * father, float value)
{
	std::list<nodeInfo*>::iterator position;
	nodeInfo* temp = new nodeInfo;
	temp->m_father = father;
	temp->m_reference = newNodo;
	temp->m_fvalue = Evaluateheuristics(&newNodo->m_position);

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
	m_reviewed.push_back(temp);
}

void CBestFS::ClearWait()
{
	m_wait.clear();
}

bool CBestFS::IsEmptyWait()
{
	return m_wait.empty();
}

nodeInfo * CBestFS::NextNodeInWait()
{
	nodeInfo* temp = m_wait.front();
	m_wait.pop_front();
	return temp;
}

CBestFS::CBestFS()
{
}

CBestFS::~CBestFS()
{
}

