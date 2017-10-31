#pragma once
#include <Windows.h>
#include <Wincodecsdk.h>

class Image
{
public:
	inline Image(int width, int height, int size, BYTE *buffer) : width(width), height(height), size(size), buffer(buffer){};
	inline ~Image() { delete[] buffer; }
	int width;
	int height;
	int size;
	BYTE *buffer;
};


