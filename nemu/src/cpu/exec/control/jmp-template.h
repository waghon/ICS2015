#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	if (op_src->type==2)
	{	
		cpu.eip=cpu.eip+op_src->val;
    	if(DATA_BYTE==2)
			cpu.eip=cpu.eip&0x0000ffff;
	}
	else if (op_src->type==0||op_src->type==1)
	{
		if(DATA_BYTE==2)
			cpu.eip=op_src->val&0x0000ffff;
		else if (DATA_BYTE==4)
			cpu.eip=op_src->val;
	}
	print_asm_template1();
}


#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE==2 ||DATA_BYTE==4

make_instr_helper(i)
make_helper(concat(jmp_rm_,SUFFIX))
{
	idex(eip,concat(decode_rm_,SUFFIX),concat(do_jmp_,SUFFIX));
	return 0;
}

#endif

#if DATA_BYTE==4

make_helper(jmp_ptr_l){
	cpu.eip=instr_fetch(eip+1,4);
	cpu.segr[CS].selector=instr_fetch(eip+5,2);
	load_segr(CS);
	return 0;
}

#endif

#include "cpu/exec/template-end.h"
