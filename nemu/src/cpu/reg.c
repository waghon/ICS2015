#include "nemu.h"
#include "special.h"
#include <stdlib.h>
#include <time.h>

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void load_segr(uint8_t sreg){
	uint32_t addr=cpu.gdtr.base+cpu.segr[sreg].INDEX*8;
	uint32_t translate[2];
	translate[0]=lnaddr_read(addr,4);
	translate[1]=lnaddr_read(addr+4,4);
	SegDesc *SD=(SegDesc *)translate;
	Assert(SD->present==1,"present = 0");
	SD_base Base;
	SD_limit Limit;
	Limit.data=0;
	Base.base_15_0=SD->base_15_0;
	Base.base_23_16=SD->base_23_16;
	Base.base_31_24=SD->base_31_24;
	Limit.limit_15_0=SD->limit_15_0;
	Limit.limit_19_16=SD->limit_19_16;
	if(SD->granularity==1)
		Limit.data=Limit.data<<12;
	cpu.segr[sreg].hidden_cache.base=Base.data;
	cpu.segr[sreg].hidden_cache.limit=Limit.data;
}

void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	uint32_t eip_sample = rand();
	cpu.eip = eip_sample;

	int i;
	for(i = R_EAX; i <= R_EDI; i ++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);

	assert(eip_sample == cpu.eip);
}

