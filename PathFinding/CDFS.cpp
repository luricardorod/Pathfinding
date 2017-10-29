#include "stdafx.h"
#include "CDFS.h"

void CDFS::InsertNodeInWait(node * newNodo, nodeInfo * father, float value)
{
	nodeInfo* temp = new nodeInfo;
	temp->m_father = father;
	temp->m_reference = newNodo;
	m_reviewed.push_back(temp);
	m_wait.push(temp);
}

void CDFS::ClearWait()
{
	while (!m_wait.empty())
	{
		m_wait.pop();
	}
}

bool CDFS::IsEmptyWait()
{
	return m_wait.empty();
}

nodeInfo * CDFS::NextNodeInWait()
{
	nodeInfo* temp = m_wait.top();
	m_wait.pop();

	return temp;
}

CDFS::CDFS()
{
}

CDFS::~CDFS()
{
}
