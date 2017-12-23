#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper(pusha_w){
	panic("pusha 16");
	return -1;
}

make_helper(pusha_l){
	uint32_t temp=cpu.esp;
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eax,SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.ecx,SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.edx,SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.ebx,SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,temp,SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.ebp,SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.esi,SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.edi,SS);
	return 1;		
}

make_helper_v(pusha)
make_helper_v(push_rm)
make_helper_v(push_r)
make_helper_v(push_i)
//make_helper_v(push_rm2r)
//make_helper_v(push_a2moffs)
//make_helper_v(push_moffs2a)
