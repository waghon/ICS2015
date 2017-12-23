#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	if(op_src->type==2)
	{
	if(DATA_BYTE==2){
		cpu.esp=cpu.esp-2;
		MEM_W(cpu.esp, (DATA_TYPE)(cpu.eip+3),SS);
		cpu.eip=(cpu.eip+op_src->val)&0x0000ffff;
	}
	else{
		cpu.esp=cpu.esp-4;
		swaddr_write(cpu.esp,4,cpu.eip+5,SS);
		cpu.eip+=op_src->val;
	}
	}
	else
	{
	if(DATA_BYTE==2){
		int len=decode_rm_w(cpu.eip+1)+1;
		cpu.esp=cpu.esp-2;
		if(op_src->type==0)
			MEM_W(cpu.esp, (DATA_TYPE)(cpu.eip+len),SS);
		else
			MEM_W(cpu.esp, (DATA_TYPE)(cpu.eip+len),SS);
		cpu.eip=(op_src->val)&0x0000ffff;
		if(op_src->type==0)
			cpu.eip-=len;
		else
			cpu.eip-=len;
	}
	else{
		int len=decode_rm_l(cpu.eip+1)+1;
		cpu.esp=cpu.esp-4;
		if(op_src->type==0)
			swaddr_write(cpu.esp,4,cpu.eip+len,SS);
		else
			swaddr_write(cpu.esp,4,cpu.eip+len,SS);
		cpu.eip=op_src->val;
		if(op_src->type==0)
			cpu.eip-=len;
		else
			cpu.eip-=len;
	} 
		
	}
//	MEM_R(cpu.esp, op_src->val);
	print_asm_template1();
}



#if DATA_BYTE==2 ||DATA_BYTE==4

make_instr_helper(i)
make_instr_helper(rm)

#endif

#include "cpu/exec/template-end.h"
