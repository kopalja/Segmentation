#include "stdafx.h"
#include "GaussianBlur.h"
#include <algorithm>



GaussianBlur::GaussianBlur()
{
}

void GaussianBlur::Process( __in double blur, __inout Image *pImage )
{
	BYTE *red = new BYTE[pImage->size / 3];
	BYTE *green = new BYTE[pImage->size / 3];
	BYTE *blue = new BYTE[pImage->size / 3];

	BYTE *red_t = new BYTE[pImage->size / 3];
	BYTE *green_t = new BYTE[pImage->size / 3];
	BYTE *blue_t = new BYTE[pImage->size / 3];

	for (int i = 0, j = 0; i < pImage->size; i += 3, j++)
	{
		red[j] = pImage->buffer[i];
		green[j] = pImage->buffer[i + 1];
		blue[j] = pImage->buffer[i + 2];
	}

	gaussBlur_4( red, red_t, pImage->width, pImage->height, blur );
	gaussBlur_4( green, green_t, pImage->width, pImage->height, blur );
	gaussBlur_4( blue, blue_t, pImage->width, pImage->height, blur );

	delete[] red;
	delete[] green;
	delete[] blue;


	for (int i = 0, j = 0; i < pImage->size; i+= 3, j++)
	{
		pImage->buffer[i] = red_t[j];
		pImage->buffer[i + 1] = green_t[j];
		pImage->buffer[i + 2] = blue_t[j];
	}

	delete[] red_t;
	delete[] green_t;
	delete[] blue_t;
}

void GaussianBlur::gaussBlur_4( __inout_ecount( pImage->size / 3 ) BYTE *scl, __inout_ecount( pImage->size / 3 ) BYTE *tcl, __in int w, __in int h, __in double r )
{
	double *pBoxes = new double[3];
	boxesForGauss( r, 3, pBoxes );
	boxBlur_4( scl, tcl, w, h, ( pBoxes[0] - 1.0 ) / 2.0 );
	boxBlur_4( tcl, scl, w, h, ( pBoxes[1] - 1.0 ) / 2.0 );
	boxBlur_4( scl, tcl, w, h, ( pBoxes[2] - 1.0 ) / 2.0 );
	delete[] pBoxes;
}

void GaussianBlur::boxBlur_4( __inout_ecount( pImage->size / 3 ) BYTE *scl, __inout_ecount( pImage->size / 3 ) BYTE *tcl, __in int w, __in int h, __in double r )
{
	for (int i = 0; i < w * h; i++) 
		tcl[i] = scl[i];
	boxBlurH_4(tcl, scl, w, h, r);
	boxBlurT_4(scl, tcl, w, h, r);
}

void GaussianBlur::boxBlurT_4( __in_ecount( pImage->size / 3 ) BYTE *pSource, __out_ecount( pImage->size / 3 ) BYTE *pTarget, __in int w, __in int h, __in double r )
{
	double iar = 1 / (r + r + 1);
	for (int i = 0; i < w; i++)
	{
		auto ti = i;
		auto li = ti;
		auto ri = ti + (int)(r * w);
		auto fv = pSource[ti];
		auto lv = pSource[ti + w * (h - 1)];
		auto val = (r + 1) * fv;
		for (auto j = 0; j < r; j++) val += pSource[ti + j * w];
		for (auto j = 0; j <= r; j++)
		{
			val += pSource[ri] - fv;
			if ((val * iar) > 255) pTarget[ti] = 255;
			else if ((val * iar) < 0) pTarget[ti] = 0;
			else pTarget[ti] = val * iar;
			ri += w;
			ti += w;
		}
		for (auto j = r + 1; j < h - r; j++)
		{
			val += pSource[ri] - pSource[li];
			if ((val * iar) > 255) pTarget[ti] = 255;
			else if ((val * iar) < 0) pTarget[ti] = 0;
			else pTarget[ti] = val * iar;
			li += w;
			ri += w;
			ti += w;
		}
		for (auto j = h - r; j < h; j++)
		{
			val += lv - pSource[li];
			if ((val * iar) > 255) pTarget[ti] = 255;
			else if ((val * iar) < 0) pTarget[ti] = 0;
			else pTarget[ti] = val * iar;
			li += w;
			ti += w;
		}
	}
}

void GaussianBlur::boxBlurH_4( __in_ecount( pImage->size / 3 ) BYTE *pSource, __out_ecount( pImage->size / 3 ) BYTE *pTarget, __in int w, __in int h, __in double r )
{
	double iar = 1 / (r + r + 1);
	for (int i = 0; i < h; i++)
	{
		auto ti = i * w;
		auto li = ti;
		auto ri = ti + (int)r;
		auto fv = pSource[ti];
		auto lv = pSource[ti + w - 1];
		auto val = (r + 1) * fv;
		for (auto j = 0; j < r; j++) val += pSource[ti + j];
		for (auto j = 0; j <= r; j++)
		{
			val += pSource[ri++] - fv;
			if ((val * iar) > 255) pTarget[ti++] = 255;
			else if ((val * iar) < 0) pTarget[ti++] = 0;
			else pTarget[ti++] = val * iar;
		}
		for (auto j = r + 1; j < w - r; j++)
		{
			val += pSource[ri++] - pTarget[li++];
			if ((val * iar) > 255) pTarget[ti++] = 255;
			else if ((val * iar) < 0) pTarget[ti++] = 0;
			else pTarget[ti++] = val * iar;
		}
		for (auto j = w - r; j < w; j++)
		{
			val += lv - pSource[li++];
			if ((val * iar) > 255) pTarget[ti++] = 255;
			else if ((val * iar) < 0) pTarget[ti++] = 0;
			else pTarget[ti++] = val * iar;
		}
	}
}

void GaussianBlur::boxesForGauss( __in double sigma, __in int n, __out_ecount( 3 ) double *pBoxes )
{
	auto wIdeal = sqrt( ( 12.0 * sigma * sigma / n ) + 1.0 );  // Ideal averaging filter width 

	auto wl = floor(wIdeal);
	
	if ((int)wl % 2 == 0) wl--;
	auto wu = wl + 2;

	auto mIdeal = (12 * sigma*sigma - n*wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
	auto m = (mIdeal);
	// var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

	for (auto i = 0; i<n; i++) 
		pBoxes[i] = (i<m ? wl : wu);
}
