struct Node;
#pragma once
#include "stdafx.h"
//#include "Node.h"
#include "Edge.h"
//#include <algorithm>
using namespace std;

class UnionTree
{
public:
	inline UnionTree(){};
	int height;
	double internalDifference;
	int count;
	double tresHold;
	static int tresHoldConstant;
};
