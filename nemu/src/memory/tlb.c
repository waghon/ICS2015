#include "nemu.h"
#include "special.h"
#include "stdlib.h"

struct{
	uint32_t volid_bit :1;
	uint32_t tag :20;
	uint32_t phy_addr :20;
}tlb[64];

void init_tlb(){
	int i=0;
	for(;i<64;i++){
		tlb[i].volid_bit=0;
	}
}

hwaddr_t add_tlb(lnaddr_t addr){
	t_12_20 cr3_addr;
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
//	assert(pte.present==1);
	if(pte.present==0){
		printf("%x %x\n",cpu.eip,addr);
		assert(0);
	}
	t_12_20 pte_addr;
	pte_addr.t_12=addr&0xfff;
	pte_addr.t_20=pte.page_frame;
	int i=0;
	for(;i<64;i++){
		if(tlb[i].volid_bit==0){
			tlb[i].tag=(addr>>12)&0xfffff;
			tlb[i].phy_addr=pte_addr.t_20;
			tlb[i].tag=1;
			return pte_addr.val;
		}
	}

	i=rand()%64;
	tlb[i].tag=(addr>>12)&0xfffff;
	tlb[i].phy_addr=pte_addr.t_20;
	tlb[i].tag=1;
	return pte_addr.val;
}

hwaddr_t read_tlb(lnaddr_t addr){
	int i=0;
	for(;i<64;i++){
		if(tlb[i].tag==((addr>>12)&0xfffff)&&tlb[i].volid_bit==1)
			return (tlb[i].phy_addr<<12)+(addr&0xfff);
	}
	return add_tlb(addr);
}
