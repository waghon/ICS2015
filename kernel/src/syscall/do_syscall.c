#include "irq.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
void serial_printc(char);
int fs_open(const char *pathname, int flags);
int fs_read(int fd, void *buf, int len);
int fs_write(int fd, void *buf, int len);
int fs_lseek(int fd, int offset, int whence);
int fs_close(int fd);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

void do_syscall(TrapFrame *tf) {
//	int fd;
//	char *buf;
//	uint32_t len;
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;
		case 3:
			tf->eax=fs_read(tf->ebx,(void *)tf->ecx,tf->edx);
			break;
		case 4:
/*			fd=tf->ebx;
			buf=(void *)tf->ecx;
			len=tf->edx;
			if(fd==1||fd==2){
//				asm volatile (".byte 0xd6" : : "a"(2), "c"(buf), "d"(len));
				int i;
				for(i=0;i<len;i++)
					serial_printc(buf[i]);
				tf->eax=len;
			}
			break;*/
			tf->eax=fs_write(tf->ebx,(void *)tf->ecx,tf->edx);
			break;
		case 5:
			tf->eax=fs_open((const char *)tf->ebx,tf->ecx);
			break;
		case 6:
			tf->eax=fs_close(tf->ebx);
			break;
		case 19:
			tf->eax=fs_lseek(tf->ebx,tf->ecx,tf->edx);
			break;
		case SYS_brk: sys_brk(tf); break;

		/* TODO: Add more system calls. */

		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

