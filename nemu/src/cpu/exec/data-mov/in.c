#include "cpu/exec/helper.h"
#include "nemu.h"

uint32_t pio_read(ioaddr_t, size_t);
void pio_write(ioaddr_t, size_t, uint32_t);


make_helper(in_i2a_b){
	int port=instr_fetch(eip+1,1);
	port=port&0xff;
	uint32_t data=pio_read(port,1);
	data=data&0xff;
	reg_b(R_EAX)=data;
	return 2;
}

make_helper(in_d2a_b){
	int port=reg_w(R_EDX);
	port=port&0xffff;
	uint32_t data=pio_read(port,1);
	data=data&0xff;
	reg_b(R_EAX)=data;
	return 1;
}

make_helper(in_i2a_w){
	int port=instr_fetch(eip+1,1);
	port=port&0xff;
	uint32_t data=pio_read(port,2);
	data=data&0xffff;
	reg_w(R_EAX)=data;
	return 2;
}

make_helper(in_i2a_l){
	int port=instr_fetch(eip+1,1);
	port=port&0xff;
	uint32_t data=pio_read(port,4);
	reg_l(R_EAX)=data;
	return 2;
}

make_helper(in_d2a_w){
	int port=reg_w(R_EDX);
	port=port&0xffff;
	uint32_t data=pio_read(port,2);
	data=data&0xffff;
	reg_w(R_EAX)=data;
	return 1;
}

make_helper(in_d2a_l){
	int port=reg_w(R_EDX);
	port=port&0xffff;
	uint32_t data=pio_read(port,4);
	reg_l(R_EAX)=data;
	return 1;
}

make_helper_v(in_i2a)
make_helper_v(in_d2a)
