#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	DATA_TYPE result = op_dest->val | op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	if(result==0)
		cpu.ZF=1;
	else
		cpu.ZF=0;

	cpu.CF=0;

	DATA_TYPE PF_s=(result>>4)^result;
	PF_s=(PF_s>>2)^PF_s;
	PF_s=(PF_s>>1)^PF_s;
	cpu.PF=(~PF_s)&0x1;

	cpu.SF=MSB(result);
	cpu.OF=0;


	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
