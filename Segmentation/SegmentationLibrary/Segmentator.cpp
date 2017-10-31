#pragma once
#include "stdafx.h"
#include "Segmentator.h"
#include <iostream>
#include <ctime>

using namespace std;
int Node::m_TresHoldConstant;



HRESULT Segmentator::Process( int argc, char *argv[] )
{
	HRESULT hr;
	ImageHandler imageHandler( argv[1], argv[2], &hr );
	if ( SUCCEEDED( hr ) )
	{
		int blur = 0;
		hr = ReadParams( argc, argv, &blur );
		if ( SUCCEEDED( hr ) )
		{
			Graph *pGraph = new Graph();
			Image *pImage = new Image( 
				imageHandler.InputImageWidth(), 
				imageHandler.InputImageHeight(), 
				imageHandler.InputImageWidth() * imageHandler.InputImageHeight() * imageHandler.InputImageByteDepth(),
				new BYTE[imageHandler.InputImageWidth() * imageHandler.InputImageHeight() * imageHandler.InputImageByteDepth()] 
			);
			hr = imageHandler.Create( pImage );
			if ( SUCCEEDED( hr ) )
			{
				GaussianBlur gaussianBlur;
				gaussianBlur.Process( pImage, blur );
				_ASSERTE( _CrtCheckMemory() );
				//
				pGraph->CreateGrid( pImage );
				_ASSERTE( _CrtCheckMemory() );
				//
				Segment( pGraph );
				_ASSERTE( _CrtCheckMemory() );
				//
				GraphToImage( pGraph->m_NumberOfVertices, pGraph->m_pVertices, pImage );
				_ASSERTE( _CrtCheckMemory() );
				//
				hr = imageHandler.Save( pImage );
				_ASSERTE( _CrtCheckMemory() );
			}
			delete pGraph;
			delete pImage;
		}
	}
	return hr;
}


HRESULT Segmentator::ReadParams( __in int argc, __in_ecount( argc ) char *argv[], __out int *pBlur )
{
	HRESULT hr = S_OK;
	if ( argc != 5 && argc != 6 ) 
		hr = E_INVALIDARG;

	if ( SUCCEEDED( hr ) )
	{
		try
		{
			*pBlur = std::stoi( argv[3] );
			Node::m_TresHoldConstant = std::stoi( argv[4] );
			//if ( argc == 6 )
				//m_SegmentMinSize = std::stoi( argv[5] );
		}
		catch ( exception )
		{
			hr = E_INVALIDARG;
		}
	}
	return hr;
}


void Segmentator::Segment( __inout Graph *pGraph )
{
	// 0. Sort Edges
	int **ppSortedEdgesIndexes = new int*[pGraph->m_NumberOfEdges];

	EdgeSort( pGraph->m_NumberOfEdges, pGraph->m_pEdges, ppSortedEdgesIndexes );
	// 1. Kazdemu vrcholu priradit vlastnu komponentu (hotovo)

	// 2. Opakuj krok 3
	int i = 0 ;
	while( *ppSortedEdgesIndexes[i] < 256 )
	{
		int index = ppSortedEdgesIndexes[i] - &pGraph->m_pEdges[0];
		int secondIndexDifference = 1;
		if ( ( index & 1 ) == 1 ) secondIndexDifference = pGraph->m_Width;

		Node *pRoot1 = &pGraph->m_pVertices[index >> 1]; // get first vertex
		while ( pRoot1->m_pPredecessor != pRoot1 ) pRoot1 = pRoot1->m_pPredecessor; // get root of segment

		Node *pRoot2 = &pGraph->m_pVertices[(index >> 1) + secondIndexDifference];
		while ( pRoot2->m_pPredecessor != pRoot2 ) pRoot2 = pRoot2->m_pPredecessor;
		
		if ( ( pRoot1 != pRoot2 ) && ( *ppSortedEdgesIndexes[i] <= pRoot1->m_TresHold) && ( *ppSortedEdgesIndexes[i] <= pRoot2->m_TresHold ) )
		{
			if ( pRoot1->m_Height > pRoot2->m_Height ) Merge( pRoot1, pRoot2, ppSortedEdgesIndexes[i] );
			else Merge( pRoot2, pRoot1, ppSortedEdgesIndexes[i] );
		}
		i++;
	}
	delete[] ppSortedEdgesIndexes;
}


void Segmentator::EdgeSort( __in int numberOfEdges, __in_ecount( numberOfEdges ) int *pEdges, __out_ecount( numberOfEdges ) int **pSortedEdgesIndexes )
{
	int count[257];
	for (int i = 0; i < 257; i++) count[i] = 0;
	int baseIndex[257];

	for (int i = 0; i < numberOfEdges; i++)
	{
		count[pEdges[i]]++;
	}
	baseIndex[0] = 0;
	for (int i = 1; i < 257; i++)
	{
		baseIndex[i] = baseIndex[i - 1] + count[i - 1];
	}
	for (int i = 0; i < numberOfEdges; i++)
	{
		pSortedEdgesIndexes[baseIndex[pEdges[i]]++] = &pEdges[i];
	}
}


inline void Segmentator::Merge( __inout Node* root1, __inout Node* root2, __in int* pEdge )
{
	root2->m_pPredecessor = root1;

	if ( root1->m_Height == root2->m_Height ) root1->m_Height++;
		root1->m_Count += root2->m_Count;

	if ( root2->m_InternalDifference > root1->m_InternalDifference) 
		root1->m_InternalDifference = root2->m_InternalDifference;
	
	if ( *pEdge > root1->m_InternalDifference) 
		root1->m_InternalDifference = *pEdge;

	root1->m_TresHold = root1->m_InternalDifference + Node::m_TresHoldConstant / root1->m_Count;
}




void Segmentator::GraphToImage( __in int numberOfVertices, __in_ecount( numberOfVertices ) Node* pVertices, __out Image *pImage )
{
	int numberOfRoots = 0;
	for (int i = 0; i < numberOfVertices; i++)
	{
		if ( pVertices[i].m_pPredecessor == &pVertices[i] ) numberOfRoots++;
	}

	Color *pColors = new Color[numberOfRoots];
	SetColorsToRoots( numberOfVertices, numberOfRoots, pVertices, pColors );
	int imageIndex = 0;

	for (int i = 0; i < numberOfVertices; i++)
	{
		Node *pRoot = &pVertices[i];
		while ( pRoot->m_pPredecessor != pRoot ) 
			pRoot = pRoot->m_pPredecessor;

		pImage->buffer[imageIndex++] = pColors[pRoot->m_ColorIndex].B;
		pImage->buffer[imageIndex++] = pColors[pRoot->m_ColorIndex].G;
		pImage->buffer[imageIndex++] = pColors[pRoot->m_ColorIndex].R;
	}
	delete[] pColors;
}



void Segmentator::SetColorsToRoots( __in int numberOfVertices, __in int numberOfRoots, __in_ecount( numberOfVertices ) Node* pVertices, __out_ecount( numberOfRoots ) Color* pColors )
{
	Node* *ppRoots = new Node*[numberOfRoots];
	int index = 0;
	for (int i = 0; i < numberOfVertices; i++)
	{
		if ( pVertices[i].m_pPredecessor == &pVertices[i] ) ppRoots[index++] = &pVertices[i];
	}
	
	int R = 0, G = 0, B = 0;
	for (int i = 0; i < numberOfRoots; i++)
	{
		pColors[i].R = R; pColors[i].G = G; pColors[i].B = B;
		R = ( R + 70 ) % 255;
		G = ( R + 20 ) % 255;
		B = ( R + G * 2 ) % 255;
		ppRoots[i]->m_ColorIndex = i;
	}
	delete[] ppRoots;
}

