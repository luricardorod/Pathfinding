#pragma once
#include "CWalker.h"
#include <stack>
class CDFS :
	public CWalker
{
public:
	virtual void InsertNodeInWait(node *newNodo, nodeInfo *father, float value = 0);
	virtual void ClearWait();
	virtual bool IsEmptyWait();
	virtual nodeInfo* NextNodeInWait();

	std::stack<nodeInfo*> m_wait;
	CDFS();
	virtual ~CDFS();
};
