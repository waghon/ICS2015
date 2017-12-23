#include "cpu/exec/template-start.h"

#define do_cmovcc(condition) \
	static void do_execute() { \
		if(condition) { \
			OPERAND_W(op_dest,op_src->val); \
		print_asm_template2(); \
		}\
	}

#define instr cmova

do_cmovcc(cpu.CF==0&&cpu.ZF==0)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovae

do_cmovcc(cpu.CF==0)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovb

do_cmovcc(cpu.CF==1)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr


	
#define instr cmovbe

do_cmovcc(cpu.CF==1||cpu.ZF==1)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr




#define instr cmove

do_cmovcc(cpu.ZF==1)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovg

do_cmovcc(cpu.ZF==0&&cpu.SF==cpu.OF)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovge

do_cmovcc(cpu.SF==cpu.OF)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovl

do_cmovcc(cpu.SF!=cpu.OF)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovle

do_cmovcc(cpu.ZF==1||cpu.SF!=cpu.OF)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovne

do_cmovcc(cpu.ZF==0)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr


#define instr cmovno

do_cmovcc(cpu.OF==0)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovnp

do_cmovcc(cpu.PF==0)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovns

do_cmovcc(cpu.SF==0)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovo

do_cmovcc(cpu.OF==1)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovp

do_cmovcc(cpu.PF==1)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr cmovs

do_cmovcc(cpu.SF==1)

#if DATA_BYTE!=1
make_instr_helper(rm2r)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr
#include "cpu/exec/template-end.h"
