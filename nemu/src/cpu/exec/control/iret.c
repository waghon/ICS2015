#include "cpu/exec/helper.h"

void load_segr(uint16_t);

make_helper(iret_w){
	panic("iret 16");
	return -1;
}

make_helper(iret_l){
	cpu.eip=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	cpu.segr[CS].selector=(uint16_t)swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	load_segr(CS);
	cpu.EFLAGS=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	return 0;
}

make_helper_v(iret)
