
#pragma once
#include "stdafx.h"
#include <algorithm>

using namespace std;

struct Color
{
	int R, G, B;
};

struct Node
{
public:
	inline Node()
	{ 
		m_pPredecessor = this;
		m_Height = 1; 
		m_Count = 1;  
		m_TresHold = Node::m_TresHoldConstant; 
		m_InternalDifference = 0;
		
	}

	inline Node* FindRoot()
	{
		Node *pAns = this;
		while ( pAns->m_pPredecessor != pAns )
		{
			pAns = pAns->m_pPredecessor;
		}
		return pAns;
	}

	Node *m_pPredecessor;

	//Index na paletu farieb (len pre korene)
	int m_ColorIndex;

	

	//UnionTreeParameters
	int m_Height;
	int m_InternalDifference;
	int m_Count;
	double m_TresHold;
	static int m_TresHoldConstant;
};

