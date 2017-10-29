#include "stdafx.h"
#include "CBFS.h"

void CBFS::InsertNodeInWait(node* newNodo, nodeInfo *father, float value)
{
	nodeInfo* temp = new nodeInfo;
	temp->m_father = father;
	temp->m_reference = newNodo;
	m_reviewed.push_back(temp);
	m_wait.push(temp);
}

void CBFS::ClearWait()
{
	while (!m_wait.empty())
	{
		m_wait.pop();
	}
}

bool CBFS::IsEmptyWait()
{
	return m_wait.empty();
}

nodeInfo* CBFS::NextNodeInWait()
{

	nodeInfo* temp = m_wait.front();
	m_wait.pop();

	return temp;
}

CBFS::CBFS()
{
}


CBFS::~CBFS()
{
}

