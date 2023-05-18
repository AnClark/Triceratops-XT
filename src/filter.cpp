
#include "filter.hpp"

//-------------------------------------------------------------------------------------------

filter::filter()
{
	type = 0;

}

//-------------------------------------------------------------------------------------------

filter::~filter()
{
}

//-------------------------------------------------------------------------------------------

void filter::init()
{
	buf0=0; buf1=0;

	f=0; pc=0; q=0;             //filter coefficients
  	bf0=0; bf1=0; bf2=0; bf3=0; bf4=0;  //filter buffers (beware denormals!)
  	t1=0; t2=0;              //temporary buffers

	selectivity=90, gain1=0, gain2=0.25, ratio=7, cap=0; 
}


//-------------------------------------------------------------------------------------------

double filter::process (double in,int type)
{
	if (in==0) { return 0; }

	double out;

	if (frequency<0) {frequency = 0; }
	if (frequency>0.6) {frequency = 0.6; }
	
	gain1 = 1.0/(selectivity + 1.0);

	cap= (in + cap*selectivity )*gain1;
	in = ((in + cap*ratio)*gain2);

	// Moog 24 dB/oct resonant lowpass VCF
	// References: CSound source code, Stilson/Smith CCRMA paper.
	// Modified by paul.kellett@maxim.abel.co.uk July 2000

	// Set coefficients given frequency & resonance [0.0...1.0]
	
	q = 1.0f - frequency;
	pc = frequency + 0.8f * frequency * q;
	f = pc + pc - 1.0f;
	q = resonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));
	
	// Filter (in [-1.0...+1.0])

	in -= q * bf4;                          //feedback
	t1 = bf1;  bf1 = (in + bf0) * pc - bf1 * f;
	t2 = bf2;  bf2 = (bf1 + t1) * pc - bf2 * f;
	t1 = bf3;  bf3 = (bf2 + t2) * pc - bf3 * f;
        bf4 = (bf3 + t1) * pc - bf4 * f;
	bf4 = bf4 - bf4 * bf4 * bf4 * 0.166667f;    //clipping
	bf0 = in;
	
	// Lowpass  output:  bf4
	// Highpass output:  in - bf4;
	// Bandpass output:  3.0f * (bf3 - bf4);

	if (type==0)
	{
    		out = bf4;
	}

	if (type==1)
	{
     		out = (3.0f * (bf3 - bf4));
	}

	if (type==2)
	{
     		out = (in - bf4);
	}

	return out;
}



