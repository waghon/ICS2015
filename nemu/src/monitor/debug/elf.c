#include "common.h"
#include <stdlib.h>
#include <elf.h>
#include "nemu.h"
char *exec_file = NULL;

static char *strtab = NULL;
static Elf32_Sym *symtab = NULL;
static int nr_symtab_entry;

static swaddr_t get_func_addr(swaddr_t eip,char *func_name)
{
	int i;
	for(i=0;i<nr_symtab_entry;i++)
	{
		if((symtab[i].st_info&0xf)==STT_FUNC)
		{
			swaddr_t func_start=symtab[i].st_value;
			swaddr_t func_end=symtab[i].st_value+symtab[i].st_size-1;
			if(eip>=func_start&&eip<=func_end)
			{
				strcpy(func_name,strtab+symtab[i].st_name);
				return func_start;	
			}
		}
	}
	return -1;
}

static void print_stack_recursion(swaddr_t ebp,swaddr_t eip)
{
	if(ebp==0)
		return;
	swaddr_t next_ebp=swaddr_read(ebp,4,SS);
	swaddr_t next_eip=swaddr_read(ebp+4,4,SS);
	print_stack_recursion(next_ebp,next_eip);
	char func_name[32];
	swaddr_t func_addr=get_func_addr(eip,func_name);
	printf("0x%08x  %s\n",func_addr,func_name);
	uint32_t args[4];
	int i;
	for(i=0;i<4;i++)
	{
		args[i]=swaddr_read(ebp+4*(i+2),4,SS);
		printf("0x%08x  %d\n",args[i],args[i]);
	}
	printf("\n");
}

void print_stack_line()
{
	swaddr_t eip=cpu.eip;
	swaddr_t esp=cpu.esp;
	swaddr_t ebp=cpu.ebp;
	char func_name[32];
	swaddr_t func_addr=get_func_addr(eip,func_name);
	uint32_t args[4];
	if(func_addr==eip)
	{
		eip=swaddr_read(esp,4,SS);
		print_stack_recursion(ebp,eip);
		printf("0x%08x  %s\n",func_addr,func_name);
		int i;
		for(i=0;i<4;i++)
		{
			args[i]=swaddr_read(esp+4*(i+1),4,SS);
			printf("0x%08x  %d\n",args[i],args[i]);
		}
	}	
	else if(func_addr==eip-1)
	{
		eip=swaddr_read(esp+4,4,SS);
		print_stack_recursion(ebp,eip);
		printf("0x%08x  %s\n",func_addr,func_name);
		int i;
		for(i=0;i<4;i++)
		{
			args[i]=swaddr_read(esp+4*(i+2),4,SS);
			printf("0x%08x  %d\n",args[i],args[i]);
		}
	}
	
	else
		print_stack_recursion(ebp,eip);
		
}

static bool match_str(int i,char *str)
{
	if((symtab[i].st_info&0xf)!=STT_FUNC&&(symtab[i].st_info&0xf)!=STT_OBJECT)
		return false;
	if(strcmp(str,strtab+symtab[i].st_name)==0)
		return true;
	return false;
}

bool find_match(char *str)
{
	int i;
	for(i=0;i<nr_symtab_entry;i++)
	{
		if(match_str(i,str)==true)
			break;
	}
	if(i!=nr_symtab_entry)
	{
		sprintf(str,"%d",symtab[i].st_value);
		return true;
	}
	return false;
}

void load_elf_tables(int argc, char *argv[]) {
	int ret;
	Assert(argc == 2, "run NEMU with format 'nemu [program]'");
	exec_file = argv[1];

	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	uint8_t buf[sizeof(Elf32_Ehdr)];
	ret = fread(buf, sizeof(Elf32_Ehdr), 1, fp);
	assert(ret == 1);

	/* The first several bytes contain the ELF header. */
	Elf32_Ehdr *elf = (void *)buf;
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	/* Load symbol table and string table for future use */

	/* Load section header table */
	uint32_t sh_size = elf->e_shentsize * elf->e_shnum;
	Elf32_Shdr *sh = malloc(sh_size);
	fseek(fp, elf->e_shoff, SEEK_SET);
	ret = fread(sh, sh_size, 1, fp);
	assert(ret == 1);

	/* Load section header string table */
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp, sh[elf->e_shstrndx].sh_offset, SEEK_SET);
	ret = fread(shstrtab, sh[elf->e_shstrndx].sh_size, 1, fp);
	assert(ret == 1);

	int i;
	for(i = 0; i < elf->e_shnum; i ++) {
		if(sh[i].sh_type == SHT_SYMTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".symtab") == 0) {
			/* Load symbol table from exec_file */
			symtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(symtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
			nr_symtab_entry = sh[i].sh_size / sizeof(symtab[0]);
		}
		else if(sh[i].sh_type == SHT_STRTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".strtab") == 0) {
			/* Load string table from exec_file */
			strtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(strtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
		}
	}

	free(sh);
	free(shstrtab);

	assert(strtab != NULL && symtab != NULL);

	fclose(fp);
}

