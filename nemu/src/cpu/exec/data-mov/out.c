#include "cpu/exec/helper.h"
#include "nemu.h"

uint32_t pio_read(ioaddr_t, size_t);
void pio_write(ioaddr_t, size_t, uint32_t);


make_helper(out_a2i_b){
	int port=instr_fetch(eip+1,1);
	port=port&0xff;
	uint32_t data=reg_b(R_EAX);
	pio_write(port,1,data);
	return 2;
}

make_helper(out_a2d_b){
	int port=reg_w(R_EDX);
	port=port&0xffff;
	uint32_t data=reg_b(R_EAX);
	pio_write(port,1,data);
	return 1;
}

make_helper(out_a2i_w){
	int port=instr_fetch(eip+1,1);
	port=port&0xff;
	uint32_t data=reg_w(R_EAX);
	pio_write(port,2,data);
	return 2;
}

make_helper(out_a2i_l){
	int port=instr_fetch(eip+1,1);
	port=port&0xff;
	uint32_t data=reg_l(R_EAX);
	pio_write(port,4,data);
	return 2;
}

make_helper(out_a2d_w){
	int port=reg_w(R_EDX);
	port=port&0xffff;
	uint32_t data=reg_w(R_EAX);
	pio_write(port,2,data);
	return 1;
}

make_helper(out_a2d_l){
	int port=reg_w(R_EDX);
	port=port&0xffff;
	uint32_t data=reg_l(R_EAX);
	pio_write(port,4,data);
	return 1;
}

make_helper_v(out_a2i)
make_helper_v(out_a2d)
