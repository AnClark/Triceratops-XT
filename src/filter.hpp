
class filter
{
public:

	filter();
	virtual ~filter();
	void init();
	double process(double,int);

	float frequency,resonance;
	int type;

private:

	float buf0,buf1;
  	float f, pc, q;             //filter coefficients
  	float bf0, bf1, bf2, bf3, bf4;  //filter buffers (beware denormals!)
  	float t1, t2;              //temporary buffers

  	float selectivity, gain1, gain2, ratio, cap; 

};
