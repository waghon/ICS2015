#include "cpu/exec/helper.h"

make_helper(stos_b)
{
	swaddr_write(cpu.edi,1,reg_b(R_AL),ES);
	if(cpu.DF==0)
		cpu.edi+=1;
	else
		cpu.edi-=1;
	print_asm("stos_b");
	return 1;
}

make_helper(stos_w)
{
	swaddr_write(cpu.edi,2,reg_w(R_AX),ES);
	if(cpu.DF==0)
		cpu.edi+=2;
	else
		cpu.edi-=2;
	print_asm("stos_w");
	return 1;
}

make_helper(stos_l)
 {
	 swaddr_write(cpu.edi,4,reg_l(R_EAX),ES);
	 if(cpu.DF==0)
		  cpu.edi+=4;
	 else
		 cpu.edi-=4;
	print_asm("stos_l");
	 return 1;
 }

make_helper_v(stos)
