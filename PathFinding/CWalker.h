#pragma once
#include "CVector2D.h"
#include <vector>

namespace HEURISTIC_TYPE
{
	enum E
	{
		kNONE,
		kQUADRATIC,
		kLINEAL,
		kMANHATTAN,
		kNUM_MAX
	};
}

struct node;
struct Conection
{
	float m_fvalue;
	node* m_node;
	Conection(float val, node* node) {
		m_fvalue = val;
		m_node = node;
	}
};

struct node
{
	int m_id;
	CVector2D m_position;
	std::vector<Conection> m_conections;
	float m_conectionValue = 0;
};


struct nodeInfo
{
	nodeInfo *m_father;
	float m_fvalue;
	node* m_reference;
};


class CWalker
{
public:
	nodeInfo m_start;
	nodeInfo m_end;
	HEURISTIC_TYPE::E m_euristicType = HEURISTIC_TYPE::kNONE;
	std::vector<node*> m_path;
	std::vector<nodeInfo*> m_reviewed;
	void SetStart(node* start);
	void SetEnd(node* end);
	void SetHeuristic(HEURISTIC_TYPE::E heuristic);
	float Evaluateheuristics(CVector2D *vec1);
	void RunPathFinding();
	virtual std::vector<node*> PathFinding();
	bool IsNodeInReviewed(node* newNodo);
	void ClearReviewed();
	virtual bool EndPathFinding(nodeInfo* temp);
	virtual void InsertChilds(nodeInfo* father);
	virtual void InsertNodeInWait(node *newNodo, nodeInfo *father, float value = 0) = 0;
	virtual void ClearWait() = 0;
	virtual bool IsEmptyWait() = 0;
	virtual nodeInfo* NextNodeInWait() = 0;
	virtual ~CWalker() {}
};
