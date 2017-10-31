#include "stdafx.h"
#include "CppUnitTest.h"
#include "Segmentator.h"
#include "Graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SegmentationTest
{		

	TEST_CLASS(SegmentatorTest)
	{
	public:
		TEST_METHOD(EdgeSortTest)
		{
			Segmentator segmentator;
			int numberOfEdges = 100;

			int* edges = new int[numberOfEdges];
			for (int i = 0; i < numberOfEdges; i++)
			{
				edges[i] = rand() % 256;
			}
			//auto sortedEdges = segmentator.EdgeSort(edges, numberOfEdges);
			for (int i = 0; i < numberOfEdges - 1; i++)
			{
				//Assert::IsTrue(*sortedEdges[i] <= *sortedEdges[i + 1]);
			}
		}
	};

}