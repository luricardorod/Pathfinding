#pragma once
#include "CWalker.h"
#include <list>

class CAStar :
	public CWalker
{
public:
	virtual void InsertNodeInWait(node *newNodo, nodeInfo *father, float value = 0);
	virtual void ClearWait();
	virtual bool IsEmptyWait();
	virtual nodeInfo* NextNodeInWait();
	virtual void InsertChilds(nodeInfo * father);
	std::list<nodeInfo*> m_wait;

	CAStar();
	virtual ~CAStar();
};

