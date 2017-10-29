#include "stdafx.h"
#include "CWalker.h"

float euristicQuadratic(CVector2D *vec1, CVector2D *vec2) {
	CVector2D temp = (*vec1 - *vec2);
	return temp.Dot(temp);
};
float euristicLineal(CVector2D *vec1, CVector2D *vec2) {
	return (*vec1 - *vec2).Magnitude();
};
float euristicManhattan(CVector2D *vec1, CVector2D *vec2) {
	return abs(vec1->x - vec2->x) + abs(vec1->y - vec2->y);
};

void CWalker::RunPathFinding()
{
	ClearWait();
	ClearReviewed();
	InsertNodeInWait(m_start.m_reference, NULL);
	nodeInfo* temp = NULL;
	bool flagPath = false;
	while (!IsEmptyWait())
	{
		temp = NextNodeInWait();
		if (EndPathFinding(temp))
		{
			flagPath = true;
			break;
		}
		InsertChilds(temp);
	}
	//si encotnro el nodo final
	if (flagPath)
	{
		m_path.clear();

		while (temp->m_father != NULL)
		{
			m_path.push_back(temp->m_reference);
			temp = temp->m_father;
		}
		m_path.push_back(temp->m_reference);
	}
}

std::vector<node*> CWalker::PathFinding()
{
	return m_path;
}

bool CWalker::IsNodeInReviewed(node* newNodo)
{
	for (auto i = m_reviewed.begin(); i != m_reviewed.end(); i++)
	{
		if ((*i)->m_reference == newNodo)
		{
			return true;
		}
	}
	return false;
}

void CWalker::InsertChilds(nodeInfo* father)
{
	for (auto conection = father->m_reference->m_conections.begin(); conection != father->m_reference->m_conections.end(); conection++)
	{
		if (!IsNodeInReviewed(conection->m_node))
		{
			InsertNodeInWait(conection->m_node, father);
		}
	}

}

void CWalker::ClearReviewed()
{
	for (auto it = m_reviewed.rbegin(); it != m_reviewed.rend(); it++)
		delete[] * it;
	m_reviewed.clear();
}

bool CWalker::EndPathFinding(nodeInfo* temp)
{
	return temp->m_reference == m_end.m_reference;
}

void CWalker::SetStart(node * start)
{
	m_start.m_reference = start;
	m_start.m_father = NULL;
}

void CWalker::SetEnd(node * end)
{
	m_end.m_reference = end;
}

void CWalker::SetHeuristic(HEURISTIC_TYPE::E heuristic)
{
	m_euristicType = heuristic;
}

float CWalker::Evaluateheuristics(CVector2D * vec1)
{
	CVector2D* vec2 = &m_end.m_reference->m_position;
	switch (m_euristicType)
	{
	case HEURISTIC_TYPE::kQUADRATIC:
		return euristicQuadratic(vec1, vec2);
	case HEURISTIC_TYPE::kLINEAL:
		return euristicLineal(vec1, vec2);
	case HEURISTIC_TYPE::kMANHATTAN:
		return euristicManhattan(vec1, vec2);
	default:
		return 0;
	}
}

