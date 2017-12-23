#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	
	if(result==0)
		cpu.ZF=1;
	else cpu.ZF=0;

	DATA_TYPE PF_s=(result>>4)^result;
PF_s=(PF_s>>2)^PF_s;
PF_s=(PF_s>>1)^PF_s;
cpu.PF=(~PF_s)&0x1;

cpu.SF=MSB(result);

if(MSB(op_src->val)==0&&MSB(op_src->val)!=MSB(result))
	cpu.OF=1;
else cpu.OF=0;

//OPERAND_W(op_dest,result);

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
