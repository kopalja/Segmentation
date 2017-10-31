#include "stdafx.h"
#include "Graph.h"



Graph::~Graph()
{
	delete[] m_pEdges;
	delete[] m_pVertices;
}


void Graph::CreateGrid( __in Image* pImage )
{
	this->m_Width = pImage->width;
	this->m_Height = pImage->height;
	InitVertexis( pImage );
	InitEdges( pImage );
}




void Graph::InitVertexis( __in Image* pImage )
{
	m_NumberOfVertices = m_Width * m_Height;
	m_pVertices = new Node[m_NumberOfVertices];
}


void Graph::InitEdges( __in Image* pImage )
{
	m_NumberOfEdges = 2 * m_NumberOfVertices;
	m_pEdges = new int[m_NumberOfEdges];
	for (int i = 0; i < m_NumberOfEdges; i++)
	{
		m_pEdges[i] = 256;
	}


	double *pGrayMap = new double[m_NumberOfVertices];
	int j = 0;
	for (int i = 0; i < m_NumberOfVertices; i++)
	{
		pGrayMap[i] = 20 * pImage->buffer[j + 2] + 69 * pImage->buffer[j + 1] + 11 * pImage->buffer[j];
		pGrayMap[i] /= 100;
		j += 3;
	}

	int decWidth = pImage->width - 1;
	int decHeight = pImage->height - 1;
	int i = 0, ei = 0;
	for (int y = 0; y < decHeight; y++)
	{
		for (int x = 0; x < decWidth; x++)
		{
			if ( pGrayMap[i] > pGrayMap[i + 1] ) m_pEdges[ei++] = pGrayMap[i] - pGrayMap[i + 1];
			else m_pEdges[ei++] = pGrayMap[i + 1] - pGrayMap[i];

			if ( pGrayMap[i] > pGrayMap[i + pImage->width] ) m_pEdges[ei++] = pGrayMap[i] - pGrayMap[i + pImage->width];
			else m_pEdges[ei++] = pGrayMap[i + pImage->width] - pGrayMap[i];

			i++;
		}
		ei++; // skipping over edge to nowhere

		if ( pGrayMap[i] > pGrayMap[i + pImage->width] ) m_pEdges[ei++] = pGrayMap[i] - pGrayMap[i + pImage->width];
		else m_pEdges[ei++] = pGrayMap[i + pImage->width] - pGrayMap[i];

		i++;
	}

	for (int x = 0; x < decWidth; x++)
	{
		if ( pGrayMap[i] > pGrayMap[i + 1] ) m_pEdges[ei++] = pGrayMap[i] - pGrayMap[i + 1];
		else m_pEdges[ei++] = pGrayMap[i + 1] - pGrayMap[i];
		ei++;
		i++;
	}
	delete[] pGrayMap;
}
