#include "cpu/exec/helper.h"


#define DATA_BYTE 2
#include "movzx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movzx-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper(movzx_rm_w2r_l)
{
	return idex(eip,decode_rm2r_w,do_movzx_l);
}

make_helper(movzx_rm_b2r_l)
{
	return idex(eip,decode_rm2r_b,do_movzx_l);
}

make_helper(movzx_rm_b2r_w)
{
	return idex(eip,decode_rm2r_b,do_movzx_w);
}

make_helper_v(movzx_rm_b2r)
