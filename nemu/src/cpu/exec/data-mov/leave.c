#include "cpu/exec/helper.h"


/* for instruction encoding overloading */

make_helper(leave_w)
{
	cpu.esp=cpu.ebp;
	reg_w(R_EBP)=swaddr_read(cpu.esp,2,SS);
	cpu.esp=cpu.esp+2;
	print_asm("leave");
	return 1;
}


make_helper(leave_l)
{
	cpu.esp=cpu.ebp;
	reg_l(R_EBP)=swaddr_read(cpu.esp,4,SS);
	cpu.esp=cpu.esp+4;
	print_asm("leave");
	return 1;
	
}
make_helper_v(leave)
