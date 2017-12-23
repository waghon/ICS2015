#include "cpu/exec/helper.h"
#include "nemu.h"

make_helper(cwt_l){
	int16_t temp=reg_w(R_EAX);
	reg_l(R_EAX)=(int)temp;
	return 1;
}

make_helper(cwt_w){
	panic("16 byte cwt");
	return -1;
}


make_helper_v(cwt);
