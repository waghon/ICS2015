#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}


#include "cpu/exec/template-end.h"
