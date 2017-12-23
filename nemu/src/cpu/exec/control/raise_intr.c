#include "nemu.h"
#include "common.h"
#include "special.h"
#include <setjmp.h>

extern jmp_buf jbuf;
void load_segr(uint16_t );

void raise_intr(uint8_t no){
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.EFLAGS,SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,(uint32_t)(cpu.segr[CS].selector&0xff),SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip,SS);
	uint32_t translate[2];
	uint32_t addr=cpu.idtr.base+no*8;
	translate[0]=lnaddr_read(addr,4);
	translate[1]=lnaddr_read(addr+4,4);
	GateDesc *GD=(GateDesc *)translate;
	Assert(GD->present==1,"idtr present 0");
	t_16_16 Base;
	Base.t_16_0=GD->offset_15_0;
	Base.t_16_1=GD->offset_31_16;
	cpu.segr[CS].selector=GD->segment;
	load_segr(CS);
	cpu.eip=Base.t_32;
	longjmp(jbuf,1);
}
