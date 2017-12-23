#include "cpu/exec/template-start.h"

#define do_setcc(condition) \
	static void do_execute() { \
		int result=0; \
		if(condition) { \
			result=1;  \
		} \
		OPERAND_W(op_src,result); \
		print_asm_template1(); \
	}

#define instr seta

do_setcc(cpu.CF==0&&cpu.ZF==0)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setae

do_setcc(cpu.CF==0)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setb

do_setcc(cpu.CF==1)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr


	
#define instr setbe

do_setcc(cpu.CF==1||cpu.ZF==1)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr




#define instr sete

do_setcc(cpu.ZF==1)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setg

do_setcc(cpu.ZF==0&&cpu.SF==cpu.OF)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setge

do_setcc(cpu.SF==cpu.OF)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setl

do_setcc(cpu.SF!=cpu.OF)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setle

do_setcc(cpu.ZF==1||cpu.SF!=cpu.OF)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setne

do_setcc(cpu.ZF==0)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr


#define instr setno

do_setcc(cpu.OF==0)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setnp

do_setcc(cpu.PF==0)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setns

do_setcc(cpu.SF==0)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr seto

do_setcc(cpu.OF==1)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr setp

do_setcc(cpu.PF==1)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr



#define instr sets

do_setcc(cpu.SF==1)

#if DATA_BYTE==1
make_instr_helper(rm)
#endif

#if DATA_BYTE!=1
  
#endif
	
#undef instr
#include "cpu/exec/template-end.h"
