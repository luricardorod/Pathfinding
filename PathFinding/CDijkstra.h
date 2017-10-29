#pragma once
#include "CWalker.h"
#include <queue>

class CDijkstra :
	public CWalker
{
public:
	virtual void InsertNodeInWait(node* newNodo, nodeInfo *father, float value = 0);
	virtual void ClearWait();
	virtual bool IsEmptyWait();
	virtual nodeInfo* NextNodeInWait();
	virtual void InsertChilds(nodeInfo* father);
	virtual std::vector<node*> PathFinding();
	std::vector<node*> CDijkstra::PathFinding(node *nodeStart);

	virtual bool EndPathFinding(nodeInfo* temp);

	std::queue<nodeInfo*> m_wait;

	CDijkstra();
	virtual ~CDijkstra();
};


