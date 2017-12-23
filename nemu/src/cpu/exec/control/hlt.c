#include "cpu/exec/helper.h"
#include "nemu.h"
//#define DATA_BYTE 1
//#include "ret-template.h"
//#undef DATA_BYTE
//void raise_intr(uint8_t );

make_helper(hlt){
	while(!(cpu.INTR & cpu.IF))
		;
	return 1;
}

/*
make_helper(int_i_l){
	int no=instr_fetch(eip+1,1);
	no=no&0xff;
	cpu.eip+=2;
	raise_intr(no);
	return 0;
}

make_helper_v(int_i)*/
