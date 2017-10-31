#pragma once
#include "Node.h"
#include "Edge.h"
#include "stdafx.h"
#include "Image.h"

using namespace std;

class Graph
{
public:
	inline Graph(){};
	~Graph();

	Node* m_pVertices;
	int* m_pEdges;
	int m_NumberOfVertices;
	int m_NumberOfEdges;
	int m_Width;
	int m_Height;


	void CreateGrid( 
		__in Image* image 
		);
private:

	void InitVertexis( 
		__in Image* pImage
		);
	void InitEdges( 
		__in Image* pImage
		);
};


