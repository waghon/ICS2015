#include "nemu.h"
#include "device/mmio.h"
uint32_t cache_1_read(hwaddr_t addr,size_t len);
void cache_1_write(hwaddr_t addr,size_t len,uint32_t data);

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
lnaddr_t seg_translate(swaddr_t ,size_t ,uint8_t);
hwaddr_t page_translate(lnaddr_t addr);
bool is_cross_doundary(lnaddr_t addr,size_t len){
	if((addr&0xfff)+len>0x1000)
		return true;
	else
		return false;
}

uint32_t cross_read(lnaddr_t addr,size_t len){
	uint32_t temp_read[2];
	int i;
	for(i=0;i<len;i++)
		*((uint32_t *)(((uint8_t *)temp_read)+i))=lnaddr_read(addr+i,1);
	return temp_read[0];

}

void cross_write(lnaddr_t addr,size_t len,uint32_t data){
	int i;
	for(i=0;i<len;i++){
		lnaddr_write(addr+i,1,*(((uint8_t *)(&data))+i));
	}		
}

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
//	printf("result %x %x\n",dram_read(addr, len) & (~0u >> ((4 - len) << 3)),cache_1_read(addr,len));
//	Assert((dram_read(addr, len) & (~0u >> ((4 - len) << 3)))==cache_1_read(addr,len),"wrong");
	int map_NO=is_mmio(addr);
	if(map_NO==-1)
		return cache_1_read(addr,len);
	else
		return mmio_read(addr,len,map_NO);
//	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int map_NO=is_mmio(addr);
	if(map_NO==-1)
		cache_1_write(addr,len,data);
	else
		mmio_write(addr,len,data,map_NO);
	//	dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	assert(len==1||len==2||len==4);
	if(is_cross_doundary(addr,len)==true)
		return cross_read(addr,len);
	hwaddr_t hwaddr=page_translate(addr);
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	assert(len==1||len==2||len==4);
	if(is_cross_doundary(addr,len)==true)
		cross_write(addr,len,data);		
	hwaddr_t hwaddr=page_translate(addr);
	hwaddr_write(hwaddr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len,uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=seg_translate(addr,len,sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data,uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=seg_translate(addr,len,sreg);
	lnaddr_write(lnaddr, len, data);
}

