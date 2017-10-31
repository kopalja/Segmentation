#include "stdafx.h"
#include "GaussianBlur.h"
#include <algorithm>



GaussianBlur::GaussianBlur()
{
}

void GaussianBlur::Process( __inout Image *image, __in double blur)
{
	BYTE *red = new BYTE[image->size / 3];
	BYTE *green = new BYTE[image->size / 3];
	BYTE *blue = new BYTE[image->size / 3];

	BYTE *red_t = new BYTE[image->size / 3];
	BYTE *green_t = new BYTE[image->size / 3];
	BYTE *blue_t = new BYTE[image->size / 3];

	for (int i = 0, j = 0; i < image->size; i += 3, j++)
	{
		red[j] = image->buffer[i];
		green[j] = image->buffer[i + 1];
		blue[j] = image->buffer[i + 2];
	}

	gaussBlur_4( red, red_t, image->width, image->height, blur );
	gaussBlur_4( green, green_t, image->width, image->height, blur );
	gaussBlur_4( blue, blue_t, image->width, image->height, blur );

	delete[] red;
	delete[] green;
	delete[] blue;


	for (int i = 0, j = 0; i < image->size; i+= 3, j++)
	{
		image->buffer[i] = red_t[j];
		image->buffer[i + 1] = green_t[j];
		image->buffer[i + 2] = blue_t[j];
	}

	delete[] red_t;
	delete[] green_t;
	delete[] blue_t;
}

void GaussianBlur::gaussBlur_4( __inout BYTE *scl, __inout BYTE *tcl, __in int w, __in int h, __in double r )
{
	auto bxs = boxesForGauss(r, 3);
	boxBlur_4(scl, tcl, w, h, (bxs[0] - 1.0) / 2.0);
	boxBlur_4(tcl, scl, w, h, (bxs[1] - 1.0) / 2.0);
	boxBlur_4(scl, tcl, w, h, (bxs[2] - 1.0) / 2.0);
	delete[] bxs;
}

void GaussianBlur::boxBlur_4( __inout BYTE *scl, __inout BYTE *tcl, __in int w, __in int h, __in double r )
{
	for (int i = 0; i < w * h; i++) tcl[i] = scl[i];
	boxBlurH_4(tcl, scl, w, h, r);
	boxBlurT_4(scl, tcl, w, h, r);
}

void GaussianBlur::boxBlurT_4( __in BYTE *scl, __out BYTE *tcl, __in int w, __in int h, __in double r )
{
	double iar = 1 / (r + r + 1);
	for (int i = 0; i < w; i++)
	{
		auto ti = i;
		auto li = ti;
		auto ri = ti + (int)(r * w);
		auto fv = scl[ti];
		auto lv = scl[ti + w * (h - 1)];
		auto val = (r + 1) * fv;
		for (auto j = 0; j < r; j++) val += scl[ti + j * w];
		for (auto j = 0; j <= r; j++)
		{
			val += scl[ri] - fv;
			if ((val * iar) > 255) tcl[ti] = 255;
			else if ((val * iar) < 0) tcl[ti] = 0;
			else tcl[ti] = val * iar;
			ri += w;
			ti += w;
		}
		for (auto j = r + 1; j < h - r; j++)
		{
			val += scl[ri] - scl[li];
			if ((val * iar) > 255) tcl[ti] = 255;
			else if ((val * iar) < 0) tcl[ti] = 0;
			else tcl[ti] = val * iar;
			li += w;
			ri += w;
			ti += w;
		}
		for (auto j = h - r; j < h; j++)
		{
			val += lv - scl[li];
			if ((val * iar) > 255) tcl[ti] = 255;
			else if ((val * iar) < 0) tcl[ti] = 0;
			else tcl[ti] = val * iar;
			li += w;
			ti += w;
		}
	}
}

void GaussianBlur::boxBlurH_4( __in BYTE *scl, __out BYTE *tcl, __in int w, __in int h, __in double r )
{
	double iar = 1 / (r + r + 1);
	for (int i = 0; i < h; i++)
	{
		auto ti = i * w;
		auto li = ti;
		auto ri = ti + (int)r;
		auto fv = scl[ti];
		auto lv = scl[ti + w - 1];
		auto val = (r + 1) * fv;
		for (auto j = 0; j < r; j++) val += scl[ti + j];
		for (auto j = 0; j <= r; j++)
		{
			val += scl[ri++] - fv;
			if ((val * iar) > 255) tcl[ti++] = 255;
			else if ((val * iar) < 0) tcl[ti++] = 0;
			else tcl[ti++] = val * iar;
		}
		for (auto j = r + 1; j < w - r; j++)
		{
			val += scl[ri++] - tcl[li++];
			if ((val * iar) > 255) tcl[ti++] = 255;
			else if ((val * iar) < 0) tcl[ti++] = 0;
			else tcl[ti++] = val * iar;
		}
		for (auto j = w - r; j < w; j++)
		{
			val += lv - scl[li++];
			if ((val * iar) > 255) tcl[ti++] = 255;
			else if ((val * iar) < 0) tcl[ti++] = 0;
			else tcl[ti++] = val * iar;
		}
	}
}

double* GaussianBlur::boxesForGauss( __in double sigma, __in int n)
{
	auto wIdeal = sqrt((12.0 * sigma * sigma / n) + 1.0);  // Ideal averaging filter width 

	auto wl = floor(wIdeal);
	
	if ((int)wl % 2 == 0) wl--;
	auto wu = wl + 2;

	auto mIdeal = (12 * sigma*sigma - n*wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
	auto m = (mIdeal);
	// var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

	double *sizes = new double[n];
	for (auto i = 0; i<n; i++) sizes[i] = (i<m ? wl : wu);
	return sizes;
}
