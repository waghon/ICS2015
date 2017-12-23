#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute()
{
	if (DATA_BYTE==2)
	{
		cpu.eip=MEM_R(cpu.esp,SS);
		cpu.esp+=2;
		cpu.eip=cpu.eip&0x0000ffff;
	}
	else
	{
		cpu.eip=MEM_R(cpu.esp,SS);
		cpu.esp+=4;	
	}
}


make_helper(concat(ret_,SUFFIX))
{
	do_execute();
	print_asm("ret");
	return 0;
}

make_helper(concat(ret_i_,SUFFIX))
{
	idex(eip,decode_i_w,do_execute);
	cpu.esp+=op_src->val;
	print_asm_template1();
	return 0;
}


#include "cpu/exec/template-end.h"
