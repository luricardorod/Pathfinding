#pragma once
#include "CWalker.h"
#include <list>
class CBestFS :
	public CWalker
{
public:
	virtual void InsertNodeInWait(node *newNodo, nodeInfo *father, float value = 0);
	virtual void ClearWait();
	virtual bool IsEmptyWait();
	virtual nodeInfo* NextNodeInWait();

	std::list<nodeInfo*> m_wait;
	CBestFS();
	virtual ~CBestFS();
};
