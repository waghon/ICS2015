#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	void ramdisk_write(uint8_t *, uint32_t, uint32_t);
	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	/* Load each program segment */
//	panic("please implement me");
	int i_phnum=0;
	for(; i_phnum<elf->e_phnum;i_phnum++ ) {
		/* Scan the program header table, load each segment into memory */
		uint8_t my_buf[64];
		ide_read(my_buf,elf->e_phoff+i_phnum*elf->e_phentsize,elf->e_phentsize);
		ph=(void *)my_buf;
		if(ph->p_type == PT_LOAD) {

 			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			 
			 
 			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
							
			uint8_t load_buf[4096];
			int i=0;
			uint32_t p_filesz=ph->p_filesz;
			uint32_t p_vaddr=mm_malloc(ph->p_vaddr,ph->p_memsz);
			uint32_t count=0;
			while(p_filesz>0)
			{
				if(p_filesz<=4096)
				{
					for(i=0;i<4096;i++)
						load_buf[i]=0;
			//		nemu_assert(ph->p_memsz<444096);
					ide_read(load_buf,ph->p_offset+count*4096,ph->p_filesz-count*4096);
					memcpy((void *)p_vaddr+count*4096,load_buf,ph->p_filesz-count*4096);
					p_filesz=0;
				}
				else
				{					
					//nemu_assert(count!=0);	
					ide_read(load_buf,ph->p_offset+count*4096,4096);
					memcpy((void *)p_vaddr+count*4096,load_buf,4096);
				//	nemu_assert(count!=30);	
					count++;
					p_filesz=p_filesz-4096;
				}
			}
			for(i=0;i<4096;i++)
				load_buf[i]=0;
			uint32_t cross=ph->p_memsz-ph->p_filesz;
			count=0;
			while(cross>0)
 			{
				if(cross<=4096)
				{				
					memcpy((void *)p_vaddr+ph->p_filesz+count*4096,load_buf,cross);
					cross=0;
 				}
				else
				{ 	
					memcpy((void *)p_vaddr+ph->p_filesz+count*4096,load_buf,4096);
					count++;
					cross=cross-4096;
				}
			}
#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
 		}
 	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
