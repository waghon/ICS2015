#include "cpu/exec/helper.h"
#include "nemu.h"

make_helper(bt_r2rm_w){
	panic("16 byte bt opcode");
	return -1;
}

make_helper(bt_r2rm_l){
	int len=decode_r2rm_l(eip+1);
	int src_data=op_src->val;
	int dest_data=op_dest->val;
	cpu.CF=(dest_data>>src_data)&0x1;
	return len+1;	
}

make_helper_v(bt_r2rm)

