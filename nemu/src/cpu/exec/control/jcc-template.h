#include "cpu/exec/template-start.h"

#define do_jcc(condition) \
	static void do_execute() { \
		if(condition) { \
			cpu.eip=cpu.eip+op_src->val; \
			if(DATA_BYTE==2) \
				cpu.eip=cpu.eip&0x0000ffff; \
		} \
		print_asm_template1(); \
	}

#define instr ja

do_jcc(cpu.CF==0&&cpu.ZF==0)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jae

do_jcc(cpu.CF==0)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jb

do_jcc(cpu.CF==1)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr


	
#define instr jbe

do_jcc(cpu.CF==1||cpu.ZF==1)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jcxz

#if DATA_BYTE!=1
do_jcc((DATA_TYPE)cpu.ecx==0)
#endif

#if DATA_BYTE==1
//make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_helper(concat(jcxz_si_,SUFFIX)){
	return idex(eip,decode_si_b,concat(do_jcxz_,SUFFIX));
}
#endif
	
#undef instr



#define instr je

do_jcc(cpu.ZF==1)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jg

do_jcc(cpu.ZF==0&&cpu.SF==cpu.OF)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jge

do_jcc(cpu.SF==cpu.OF)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jl

do_jcc(cpu.SF!=cpu.OF)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jle

do_jcc(cpu.ZF==1||cpu.SF!=cpu.OF)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jne

do_jcc(cpu.ZF==0)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr


#define instr jno

do_jcc(cpu.OF==0)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jnp

do_jcc(cpu.PF==0)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jns

do_jcc(cpu.SF==0)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jo

do_jcc(cpu.OF==1)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr jp

do_jcc(cpu.PF==1)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr



#define instr js

do_jcc(cpu.SF==1)

#if DATA_BYTE==1
make_instr_helper(si)
#endif

#if DATA_BYTE!=1
make_instr_helper(i)
#endif
	
#undef instr
#include "cpu/exec/template-end.h"
