#pragma once
#include "CWalker.h"
#include <queue>
class CBFS :
	public CWalker
{
public:

	virtual void InsertNodeInWait(node *newNodo, nodeInfo *father, float value = 0);
	virtual void ClearWait();
	virtual bool IsEmptyWait();
	virtual nodeInfo* NextNodeInWait();

	std::queue<nodeInfo*> m_wait;

	CBFS();
	virtual ~CBFS();
};

