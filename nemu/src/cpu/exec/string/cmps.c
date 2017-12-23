#include "cpu/exec/helper.h"

make_helper(cmps_b)
{
	uint8_t op_src1=swaddr_read(cpu.esi,1,DS);
	uint8_t op_dest1=swaddr_read(cpu.edi,1,ES);
	uint8_t result=op_src1-op_dest1;

	if(result==0)
		cpu.ZF=1;
	else cpu.ZF=0;

	if(op_src1 < op_dest1)
		cpu.CF=1;
	else 
		cpu.CF=0;

	uint8_t PF_s=(result>>4)^result;
	PF_s=(PF_s>>2)^PF_s; 
	PF_s=(PF_s>>1)^PF_s; 
	cpu.PF=(~PF_s)&0x1;

	cpu.SF=result>>7; 

	if((op_dest1>>7)!=(op_src1>>7)&&(op_src1>>7)!=(result>>7))
		cpu.OF=1;
	else cpu.OF=0; 

	if(cpu.DF==0)
	{
		cpu.edi+=1;
		cpu.esi+=1;
	}
	else
	{
		cpu.edi-=1;
		cpu.esi-=1;
	}
	print_asm("cmps_b");
	return 1;
}

make_helper(cmps_w)
{
	uint16_t op_src1=swaddr_read(cpu.esi,2,DS);
	uint16_t op_dest1=swaddr_read(cpu.edi,2,ES);
	uint16_t result=op_src1-op_dest1;

	if(result==0)
		cpu.ZF=1;
	else cpu.ZF=0;

	if(op_src1 < op_dest1)
		cpu.CF=1;
	else 
		cpu.CF=0;

	uint16_t PF_s=(result>>4)^result;
	PF_s=(PF_s>>2)^PF_s; 
	PF_s=(PF_s>>1)^PF_s; 
	cpu.PF=(~PF_s)&0x1;

	cpu.SF=result>>15; 

	if((op_dest1>>15)!=(op_src1>>15)&&(op_src1>>15)!=(result>>15))
		cpu.OF=1;
	else cpu.OF=0;

	if(cpu.DF==0)
	{
		cpu.edi+=2;
		cpu.esi+=2;
	}
	else
	{
		cpu.edi-=2;
		cpu.esi-=2;
	}
	print_asm("cmps_w");
	return 1;
}

make_helper(cmps_l)
 {
	uint32_t op_src1=swaddr_read(cpu.esi,4,DS);
	uint32_t op_dest1=swaddr_read(cpu.edi,4,ES);
	uint32_t result=op_src1-op_dest1;

	if(result==0)
		cpu.ZF=1;
	else cpu.ZF=0;

	if(op_src1 < op_dest1)
		cpu.CF=1;
	else 
		cpu.CF=0;

	uint32_t PF_s=(result>>4)^result;
	PF_s=(PF_s>>2)^PF_s; 
	PF_s=(PF_s>>1)^PF_s; 
	cpu.PF=(~PF_s)&0x1;

	cpu.SF=result>>31; 

	if((op_dest1>>31)!=(op_src1>>31)&&(op_src1>>31)!=(result>>31))
		cpu.OF=1;
	else cpu.OF=0;

	 if(cpu.DF==0)
	 {
		  cpu.edi+=4;
		  cpu.esi+=4;
	 }
	 else
	 {
		 cpu.edi-=4;
		 cpu.esi-=4;
	 }
	print_asm("cmps_l");
	 return 1;
 }

make_helper_v(cmps)
