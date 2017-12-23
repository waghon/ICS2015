#include "cpu/exec/template-start.h"

#define instr lidt

static void do_execute() {
	cpu.idtr.limit=swaddr_read(op_src->addr,2,DS);
	if(DATA_BYTE==2)
		cpu.idtr.base=swaddr_read(op_src->addr+2,3,DS);
	else
		cpu.idtr.base=swaddr_read(op_src->addr+2,4,DS);

	print_asm_template1();
}

make_instr_helper(rm)


#include "cpu/exec/template-end.h"
