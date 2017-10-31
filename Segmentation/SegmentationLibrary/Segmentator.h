
#include "stdafx.h"
#include "ImageHandler.h"
#include "Graph.h"
#include "GaussianBlur.h"

using namespace std;

#pragma once
class Segmentator
{
public:
	HRESULT Process( 
		__in int argc,
		__in_ecount( argc ) char *argv[] 
	);

private:

	HRESULT ReadParams( 
		__in int argc,
		__in_ecount( argc ) char *argv[],
		__out int *pBlur
	);

	void Segment( 
		__inout Graph* pGraph 
		);

	void EdgeSort( 
		__in int numberOfEdges, 
		__in_ecount( numberOfEdges ) int *pEdges, 
		__out_ecount( numberOfEdges ) int **pSortedEdgesIndexes 
		);

	inline void Merge( 
		__inout Node* pRoot1, 
		__inout Node* pRoot2,
		__in int* pEdge 
		);

	void GraphToImage( 
		__in int numberOfVertices, 
		__in_ecount( numberOfVertices ) Node* pVertices,
		__out Image *pImage 
		);

	void SetColorsToRoots( 
		__in int numberOfVertices, 
		__in int numberOfRoots, 
		__in_ecount( numberOfVertices ) Node* pVertices, 
		__out_ecount( numberOfRoots ) Color* pColors 
		);
};

