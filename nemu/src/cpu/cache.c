#include "nemu.h"
#include "common.h"
#include <stdlib.h>

#define NR_DATA 64

typedef struct{
	struct {
		unsigned dirty_bit:1;
		unsigned volid_bit:1;
	};
	uint32_t addr_extra;
	uint8_t data[NR_DATA];
}Cache;


uint32_t cache_1_read(hwaddr_t addr,size_t len);
uint32_t cache_2_read(hwaddr_t addr,size_t len);
void cache_1_write(hwaddr_t addr,size_t len,uint32_t data);
void cache_2_write(hwaddr_t addr,size_t len,uint32_t data);
void init_cache_1();
void init_cache_2();
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
bool cache_1read_group(hwaddr_t addr,size_t len,uint32_t *ans);
void cache_1add_group(hwaddr_t addr);
bool cache_1write_group(hwaddr_t addr,size_t len,uint32_t data);
bool cache_2read_group(hwaddr_t addr,size_t len,uint32_t *ans);
void cache_2add_group(hwaddr_t addr);
bool cache_2write_group(hwaddr_t addr,size_t len,uint32_t data);

//#define MY_DEBUG

#define ALL_BITS 32

#define NR_LINES_1 1024
#define SZ_GROUP_1 8
#define WRITE_BACK_1 0
#define WRITE_ALLOCATE_1 0

#define NR_LINES_2 65536
#define SZ_GROUP_2 16
#define WRITE_BACK_2 1
#define WRITE_ALLOCATE_2 1

Cache cache_1[SZ_GROUP_1][NR_LINES_1/SZ_GROUP_1];
Cache cache_2[SZ_GROUP_2][NR_LINES_2/SZ_GROUP_2];

void init_cache(){
	init_cache_1();
	init_cache_2();
}

#define CACHE_1

#include "cache-template.h"

#undef CACHE_1


#define CACHE_2

#include "cache-template.h"

#undef CACHE_2
