// Segmentation.cpp : Defines the entry point for the console application.
//
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>


#include "stdafx.h"
#include "Segmentator.h"

int main(int argc, char* argv[])
{

	Segmentator s;
	HRESULT hr = s.Process(argc, argv);

	cout << hr << endl;

	_CrtDumpMemoryLeaks();



	return 0;
}

