#include "cpu/exec/helper.h"

make_helper(movs_b)
{
	swaddr_write(cpu.edi,1,swaddr_read(cpu.esi,1,DS),ES);
	if(cpu.DF==0)
	{
		cpu.edi+=1;
		cpu.esi+=1;
	}
	else
	{
		cpu.edi-=1;
		cpu.esi-=1;
	}
	print_asm("movs_b");
	return 1;
}

make_helper(movs_w)
{
	swaddr_write(cpu.edi,2,swaddr_read(cpu.esi,2,DS),ES);
	if(cpu.DF==0)
	{
		cpu.edi+=2;
		cpu.esi+=2;
	}
	else
	{
		cpu.edi-=2;
		cpu.esi-=2;
	}
	print_asm("movs_w");
	return 1;
}

make_helper(movs_l)
 {
	 swaddr_write(cpu.edi,4,swaddr_read(cpu.esi,4,DS),ES);
	 if(cpu.DF==0)
	 {
		  cpu.edi+=4;
		  cpu.esi+=4;
	 }
	 else
	 {
		 cpu.edi-=4;
		 cpu.esi-=4;
	 }
	 print_asm("movs_l");
	 return 1;
 }

make_helper_v(movs)
