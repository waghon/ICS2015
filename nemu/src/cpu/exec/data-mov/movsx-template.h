#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	if(op_src->size==1)
	{
		op_src->val=(((int)op_src->val)<<24)>>24;
	}
	else
		op_src->val=(((int)op_src->val)<<16)>>16;
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}


#include "cpu/exec/template-end.h"
