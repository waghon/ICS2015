#include "nemu.h"
#include "special.h"

hwaddr_t read_tlb(lnaddr_t addr);

lnaddr_t seg_translate(swaddr_t addr,size_t len,uint8_t sreg){
	lnaddr_t ret_addr;
	if(cpu.PE==1){
		ret_addr=cpu.segr[sreg].hidden_cache.base;
		ret_addr+=addr;
		Assert(addr<=cpu.segr[sreg].hidden_cache.limit,"Sreg over limit");
	}
	else
		ret_addr=addr;
	return ret_addr;
}

hwaddr_t page_translate(lnaddr_t addr){
	if(cpu.PE==1&&cpu.PG==1){
	/*	t_12_20 cr3_addr;
		t_12_10_10 t_12_10_10_addr;
		t_12_10_10_addr.t_10_l=0;
		t_12_10_10_addr.t_10_h=0;
		t_12_10_10_addr.val=addr;
		cr3_addr.val=0;
		cr3_addr.t_20=cpu.CR3.page_directory_base;
		PDE pde;
		pde.val=hwaddr_read(cr3_addr.val+t_12_10_10_addr.t_10_h*4,4);
		assert(pde.present==1);
		t_12_20 pde_addr;
		pde_addr.val=0;
		pde_addr.t_20=pde.page_frame;
		PTE pte;
		pte.val=hwaddr_read(pde_addr.val+t_12_10_10_addr.t_10_l*4,4);
		assert(pte.present==1);
		t_12_20 pte_addr;
		pte_addr.t_12=addr&0xfff;
		pte_addr.t_20=pte.page_frame;
//		printf("%x\n",pte_addr.val);
		return pte_addr.val;	*/
		return read_tlb(addr);
	}
	
	return addr;
}
