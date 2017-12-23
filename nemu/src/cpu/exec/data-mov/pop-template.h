#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	if(DATA_BYTE==2){
		OPERAND_W(op_src, MEM_R(cpu.esp,SS));
		cpu.esp=cpu.esp+2;
	}
	else{
		OPERAND_W(op_src, MEM_R(cpu.esp,SS));
		cpu.esp=cpu.esp+4; 
	}
//	MEM_R(cpu.esp, op_src->val);
	print_asm_template1();
}

make_instr_helper(rm)
make_instr_helper(r)


#include "cpu/exec/template-end.h"
