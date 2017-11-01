#pragma once
#include "Node.h"
#include "Edge.h"
#include "stdafx.h"
#include "Image.h"

using namespace std;

class Graph
{
public:
	Graph( 
		__in Image *pImage 
	);

	~Graph();

	Node* m_pVertices;
	int* m_pEdges;

	int m_NumberOfVertices;
	int m_Width;


	void CreateGrid( 
		__in Image* image 
		);
private:

	void InitEdges( 
		__in Image* pImage
		);
};


