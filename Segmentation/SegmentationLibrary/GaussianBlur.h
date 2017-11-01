#pragma once
#include "Image.h"

class GaussianBlur
{
public:
	GaussianBlur();
	void Process( 
		__in double blur,
		__inout Image *pImage
		);

private:
	inline void gaussBlur_4( 
		__inout_ecount( pImage->size / 3 ) BYTE *scl, 
		__inout_ecount( pImage->size / 3 ) BYTE *tcl, 
		__in int w, 
		__in int h, 
		__in double r 
		);

	inline void boxBlur_4( 
		__inout_ecount( pImage->size / 3 ) BYTE *scl, 
		__inout_ecount( pImage->size / 3 ) BYTE *tcl, 
		__in int w, 
		__in int h, 
		__in double r 
		);

	inline void boxBlurT_4( 
		__in_ecount( pImage->size / 3 ) BYTE *pSource, 
		__out_ecount( pImage->size / 3 ) BYTE *pTarget, 
		__in int w, 
		__in int h, 
		__in double r 
		);

	inline void boxBlurH_4( 
		__in_ecount( pImage->size / 3 ) BYTE *pSource, 
		__out_ecount( pImage->size / 3 ) BYTE *pTarget, 
		__in int w, 
		__in int h, 
		__in double r 
		);

	inline void boxesForGauss( 
		__in double sigma,
		__in int n,
		__out_ecount( 3 ) double *pBoxes
		);
};

