#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	union{
	union {
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];
	} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */
struct{
	uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;};
	};
	swaddr_t eip;
	union{
		struct
		{
			unsigned CF:1;
			unsigned PF:1;
			unsigned ZF:1;
			unsigned SF:1;
			unsigned IF:1;
			unsigned DF:1;
			unsigned OF:1;
			unsigned UNUSE:25;
		};
		uint32_t EFLAGS;
	};
	struct{
		uint32_t base;
		uint16_t limit;
	}gdtr;
	struct{
		uint32_t base;
		uint16_t limit;
	}idtr;
	union{
		struct{
			unsigned PE:1;
			unsigned MP:1;
			unsigned EM:1;
			unsigned TS:1;
			unsigned ET:1;
			unsigned RESERVED:26;
			unsigned PG:1;
		};
		uint32_t CR0;
	};
	union {
		struct {
			uint32_t pad0:3;
			uint32_t page_write_through:1;
			uint32_t page_cache_disable:1;
			uint32_t pad1:7;
			uint32_t page_directory_base:20;
		};
		uint32_t val;
	}CR3;
	volatile bool INTR;
	struct{
		union{
			struct{
				unsigned RPL:2;
				unsigned TI:1;
				unsigned INDEX:13;
			};
			uint16_t selector;
		};
		struct{
			uint32_t base;
			uint32_t limit;
		}hidden_cache;
	}segr[4];
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
