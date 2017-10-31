#pragma once
#include "Image.h"

class GaussianBlur
{
public:
	GaussianBlur();
	void Process( __inout Image *image, __in double blur );

private:
	inline void gaussBlur_4( __inout BYTE *scl, __inout BYTE *tcl, __in int w, __in int h, __in double r );

	inline void boxBlur_4( __inout BYTE *scl, __inout BYTE *tcl, __in int w, __in int h, __in double r );

	inline void boxBlurT_4( __in BYTE *scl, __out BYTE *tcl, __in int w, __in int h, __in double r );

	inline void boxBlurH_4( __in BYTE *scl, __out BYTE *tcl, __in int w, __in int h, __in double r );

	inline double* boxesForGauss( __in double sigma, __in int n);
};

