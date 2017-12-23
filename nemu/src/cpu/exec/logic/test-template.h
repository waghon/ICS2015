#include "cpu/exec/template-start.h"

#define instr test

static void do_execute()
{
	//DATA_TYPE op_src1_s=(DATA_TYPE)op_src->val;
	DATA_TYPE op_src1=(DATA_TYPE)op_src->val;
	DATA_TYPE result=(((DATA_TYPE)op_dest->val)&op_src1);	
//	printf("%d %d %d  ",op_src1,op_dest->val,result);

	if(result==0)
	   	cpu.ZF=1;
	else
	   	cpu.ZF=0;

/*	if(op_dest->val < op_src1)
		cpu.CF=1;
	else
		cpu.CF=0;*/

	cpu.CF=0;

	DATA_TYPE PF_s=(result>>4)^result;
	PF_s=(PF_s>>2)^PF_s;
	PF_s=(PF_s>>1)^PF_s;
	cpu.PF=(~PF_s)&0x1;

	cpu.SF=MSB(result);

/*	if(MSB(op_dest->val)==MSB(-op_src1)&&MSB(op_dest->val)!=MSB(result))
		cpu.OF=1;
	else cpu.OF=0;*/

	cpu.OF=0;

//	OPERAND_W(op_dest,result);

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE==2 || DATA_BYTE==4

make_instr_helper(si2rm)

#endif

#include "cpu/exec/template-end.h"
