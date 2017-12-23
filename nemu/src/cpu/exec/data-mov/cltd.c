#include "cpu/exec/helper.h"


/* for instruction encoding overloading */

make_helper(cltd_w)
{
	if(reg_w(R_AX)>>15)
		reg_w(R_DX)=0xffff;
	else
		reg_w(R_DX)=0;
	print_asm("cltdw");
	return 1;
}

make_helper(cltd_l)
{ 
	if(cpu.eax>>31)
		cpu.edx=0xffffffff;
	else
		cpu.edx=0;
	print_asm("cltdl");
	return 1;
}
make_helper_v(cltd)
