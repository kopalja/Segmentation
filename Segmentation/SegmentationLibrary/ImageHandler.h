#pragma once
#include "stdafx.h"
#include "Image.h"
#include <Windows.h>
#include <Wincodecsdk.h>
#include<string>

using namespace std;

enum Format
{
	Jpeg, Tiff, Wrong
};

class ImageHandler
{
public:
	ImageHandler( 
		__in char* inputPath, 
		__in char* outputPath, 
		__out HRESULT* hr
		);

	HRESULT Create(
		__out Image* image 
		);

	HRESULT Save( 
		__in Image *image 
		);

	inline UINT InputImageWidth(){ return width; }
	inline UINT InputImageHeight(){ return height; }
	inline UINT InputImageByteDepth(){ return byteDepth; }

private:

	HRESULT DefineFormat( 
		__in char* inputPath 
		);

	HRESULT SetPaths( 
		__in_ecount( 255 ) char* inputPath, 
		__in_ecount( 255 ) char* outputPath, 
		__out wchar_t *convertedInputPath, 
		__out wchar_t *convertedOutputPath 
		);

	HRESULT SetCoders( 
		__in LPCWSTR inputPath, 
		__in LPCWSTR outputPath 
		);

	HRESULT SetFrames(
		);

	Format format;
	double dpiX, dpiY;
	UINT width, height, byteDepth;
	IWICBitmapDecoder *piDecoder;
	IWICBitmapEncoder *piEncoder;
	WICPixelFormatGUID pixelFormat;
	IWICMetadataBlockReader *piBlockReader;
	IWICMetadataBlockWriter *piBlockWriter;
	IWICStream *piFileStream;
	IWICBitmapFrameDecode *piFrameDecode;
	IWICBitmapFrameEncode *piFrameEncode;
	IWICMetadataQueryReader *piFrameQReader;
	IWICMetadataQueryWriter *piFrameQWriter;
};

