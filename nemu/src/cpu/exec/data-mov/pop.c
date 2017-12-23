#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper(popa_w){
	panic("popa 16");
	return -1;
}

make_helper(popa_l){
	cpu.edi=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	cpu.esi=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	cpu.ebp=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
//	cpu.edi=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	cpu.ebx=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	cpu.edx=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	cpu.ecx=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	cpu.eax=swaddr_read(cpu.esp,4,SS);
	cpu.esp+=4;
	return 1;	
}

make_helper_v(popa)
make_helper_v(pop_rm)
make_helper_v(pop_r)
