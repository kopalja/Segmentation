// Segmentation.cpp : Defines the entry point for the console application.
//
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <tuple>
#include "stdafx.h"
#include "Segmentator.h"
#include <iostream>
#include <functional>

std::tuple< int, int > ReadParams( int argc, char* argv[] )
{
	int blur = 0;
	int tresHold = 0;

	if ( argc != 5 ) 
		return std::make_tuple( -1, -1 );

	try
	{
		blur = std::stoi( argv[3] );
		tresHold = std::stoi( argv[4] );
	}
	catch ( exception )
	{
		return std::make_tuple( -1, -1 );
	}

	if ( blur < 0 || tresHold < 0 )
		return std::make_tuple( -1, -1 );

	return std::make_tuple( blur, tresHold );
}

void PrintError()
{
	std::cout << "Wrong parameters " << endl;
	cout << "Correct input :  Segmentation.exe < InputPath, OutputPath, blur( positive number ), tresHold( positive number ) >" << endl;
}

int main(int argc, char* argv[])
{
	auto params = ReadParams( argc, argv );

	if ( std::get<0>( params ) == -1 )
	{
		PrintError();
		return 0;
	}


	Segmentator s;
	HRESULT hr = s.Process( argv[1], argv[2], std::get<0>( params ), std::get<1>( params ) );
	if ( SUCCEEDED( hr ) )
	{
		cout << "done" << endl;
	}
	else
	{
		cout << "Wrong input or output image path" << endl;
	}
	_CrtDumpMemoryLeaks();


	return 0;
}

