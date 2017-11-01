#include "stdafx.h"
#include "Graph.h"


Graph::Graph( __in Image *pImage )
{
	this->m_Width = pImage->width;
	m_NumberOfVertices = pImage->width * pImage->height;

	m_pVertices = new Node[m_NumberOfVertices];
	m_pEdges = new int[m_NumberOfVertices * 2];

	InitEdges( pImage );
}


Graph::~Graph()
{
	delete[] m_pEdges;
	delete[] m_pVertices;
}







void Graph::InitEdges( __in Image* pImage )
{
	/* Make all edges invalid */
	UINT numberOfEdges = m_NumberOfVertices * 2;
	for (int i = 0; i < numberOfEdges; i++)
	{
		m_pEdges[i] = 256;
	}


	/* Greate grayscale bitmap of image */
	double *pGrayMap = new double[m_NumberOfVertices];
	int j = 0;
	for (int i = 0; i < m_NumberOfVertices; i++)
	{
		pGrayMap[i] = 20 * pImage->buffer[j + 2] + 69 * pImage->buffer[j + 1] + 11 * pImage->buffer[j];
		pGrayMap[i] /= 100;
		j += 3;
	}


	/* Assign valid values to edges */
	int decWidth = pImage->width - 1;
	int decHeight = pImage->height - 1;
	int i = 0, ei = 0;
	for (int y = 0; y < decHeight; y++)
	{
		for (int x = 0; x < decWidth; x++)
		{
			if ( pGrayMap[i] > pGrayMap[i + 1] ) 
				m_pEdges[ei++] = pGrayMap[i] - pGrayMap[i + 1];
			else 
				m_pEdges[ei++] = pGrayMap[i + 1] - pGrayMap[i];

			if ( pGrayMap[i] > pGrayMap[i + pImage->width] ) 
				m_pEdges[ei++] = pGrayMap[i] - pGrayMap[i + pImage->width];
			else 
				m_pEdges[ei++] = pGrayMap[i + pImage->width] - pGrayMap[i];

			i++;
		}
		ei++; // skipping over edge to nowhere

		if ( pGrayMap[i] > pGrayMap[i + pImage->width] )
			m_pEdges[ei++] = pGrayMap[i] - pGrayMap[i + pImage->width];
		else 
			m_pEdges[ei++] = pGrayMap[i + pImage->width] - pGrayMap[i];

		i++;
	}

	for (int x = 0; x < decWidth; x++)
	{
		if ( pGrayMap[i] > pGrayMap[i + 1] ) 
			m_pEdges[ei++] = pGrayMap[i] - pGrayMap[i + 1];
		else 
			m_pEdges[ei++] = pGrayMap[i + 1] - pGrayMap[i];
		ei++;
		i++;
	}
	delete[] pGrayMap;
}
