#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
//	nemu_assert(0);
	int sign=((a>>31)^(b>>31))&0x1;
	if(a<0)
		a=-a;
	if(b<0)
		b=-b;
	int ha=(a>>16)&0xffff;
	int hb=(b>>16)&0xffff;
	int la=a&0xffff;
	int lb=b&0xffff;
	FLOAT result=((ha*hb)<<16)+(la*hb+lb*ha)+(((la*lb)>>16)&0xffff)+(((la*lb)&0xffff)>=0x8000);
	if(sign==1)
		result=-result;
	return result;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	//nemu_assert(0);
	int sign=((a>>31)^(b>>31))&0x1;
	if(a<0)
		a=-a;
	if(b<0)
		b=-b;
	int i=0;
	FLOAT result_h=a/b;
	FLOAT remainder=a%b;
	for(;i<16;i++)
	{
		result_h=(result_h<<1)+(remainder<<1)/b;
		remainder=(remainder<<1)%b;
		if(i==15&&(remainder*2)>=b)
			result_h++;
	}
	if(sign==1)
		result_h=-result_h;
	return result_h;
}

FLOAT f2F(float a) {
//	nemu_assert(0);
	char *p=(char *)(&a);
	int i;
	int result=0;
	for(i=3;i>=0;i--)
	{
		result=result<<8;		
		result=result+((*(p+i))&0xff);
	}
	int sign=(result>>31)&0x1;
	int h=(result>>23)&0xff;
	int l=result&0x7fffff;
	if(h!=0)
		l+=0x1<<23;
	int ss=h-127;
	if(h==0)
		ss++;
	ss=ss-7;
	if(ss>0)
		l=l<<ss;
	else
		l=l>>(-ss);
	if(sign==1)
		l=-l;
	return l;
}

FLOAT Fabs(FLOAT a) {
//	nemu_assert(0);
	if(a<0)
		a=-a;
	return a;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

