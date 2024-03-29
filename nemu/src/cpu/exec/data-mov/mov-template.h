#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX),DS);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr,DS);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE==2

make_helper(mov_rm2segr_w){
	size_t len=decode_rm2r_w(cpu.eip+1);
	cpu.segr[op_dest->reg].selector=op_src->val;
	load_segr(op_dest->reg);
	return len+1;
}

#endif

#if DATA_BYTE==4	

make_helper(mov_cr2r_l){
	decode_r2rm_l(cpu.eip+2);
	if(op_src->reg==0){
		REG(op_dest->reg)=cpu.CR0;
	}
	else if(op_src->reg==3){
		REG(op_dest->reg)=cpu.CR3.val;
	}
	print_asm_template2();
	return 2;
}

make_helper(mov_r2cr_l){
	decode_rm2r_l(cpu.eip+2);
	if(op_dest->reg==0){
		cpu.CR0=REG(op_src->reg);
	}
	else if(op_dest->reg==3){
		cpu.CR3.val=REG(op_src->reg);
		init_tlb();
	}
	print_asm_template2();
	return 2;
	
}

#endif

#include "cpu/exec/template-end.h"
