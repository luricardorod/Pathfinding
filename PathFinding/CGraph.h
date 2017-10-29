#pragma once
#include <vector>
#include "CWalker.h"
class CGraph
{
public:
	std::vector<node> m_graph;
	void CreateGraph(int rows, int columns, float distanceX, float distanceY);

	node* m_start;
	node* m_end;
	void SetStart(node* start);
	void SetEnd(node* end);
	void DisconectNode(node* node);
	void ChangeValueConnections(node* node, float value);
	CGraph();
	~CGraph();
};

