#ifndef FUC_DEFINE
#define FUC_DEFINE

uint32_t get_power_2(uint32_t num){
	uint32_t count=0;
 	while(num>1){
		num/=2;
		count++;
	}
	return count;
}

#endif

#ifdef CACHE_1

#undef CACHE
#define CACHE cache_1

#undef NR_LINES
#define NR_LINES NR_LINES_1

#undef SZ_GROUP
#define SZ_GROUP SZ_GROUP_1

#undef WRITE_BACK
#define WRITE_BACK WRITE_BACK_1

#undef WRITE_ALLOCATE
#define WRITE_ALLOCATE WRITE_ALLOCATE_1

#endif


#ifdef CACHE_2

#undef CACHE
#define CACHE cache_2

#undef NR_LINES
#define NR_LINES NR_LINES_2

#undef SZ_GROUP
#define SZ_GROUP SZ_GROUP_2

#undef WRITE_BACK
#define WRITE_BACK WRITE_BACK_2

#undef WRITE_ALLOCATE
#define WRITE_ALLOCATE WRITE_ALLOCATE_2

#endif



#ifndef FUNC_CACHE
#define FUNC_CACHE

#define get_1_all(num) \
	((~0u)>>(32-(num)))

#define get_inline_addr(addr) \
	(addr&get_1_all(get_power_2(NR_DATA)))

#define get_group_no(addr) \
	 ((addr>>(get_power_2(NR_DATA)))&get_1_all(get_power_2(NR_LINES/SZ_GROUP)))

#define get_in_group_no(addr) \
	 ((addr>>((get_power_2(NR_DATA))+get_power_2(NR_LINES/SZ_GROUP)))&get_1_all(get_power_2(NR_LINES)))

#define get_extra_addr(addr) \
	 ((addr>>(get_power_2(NR_DATA)+get_power_2(NR_LINES/SZ_GROUP)))&get_1_all(ALL_BITS-get_power_2(NR_DATA*NR_LINES/SZ_GROUP)))

#endif

void concat(init_,CACHE)(){
	int i,j;
 	for(i=0;i<SZ_GROUP;i++){
 		for(j=0;j<NR_LINES/SZ_GROUP;j++){
			CACHE[i][j].volid_bit=0;
		}
	}
}


bool concat(CACHE,read_group)(hwaddr_t addr,size_t len,uint32_t *ans){
	uint32_t group_no=get_group_no(addr);
	uint32_t inline_addr=get_inline_addr(addr);
	uint32_t extra_addr=get_extra_addr(addr);
	
	int i;
#ifdef MY_DEBUG
	printf("read group %x %x %x\n",addr,SZ_GROUP,group_no);
#endif
	for(i=0;i<SZ_GROUP;i++){
		if(CACHE[i][group_no].volid_bit==1&&CACHE[i][group_no].addr_extra==extra_addr){
			if(NR_DATA-inline_addr<len){
				uint32_t temp_read[2];
				if(concat(CACHE,read_group)(addr+len-1,4,temp_read)==false)
					concat(CACHE,add_group)(addr+len-1);
				concat(CACHE,read_group)(addr,NR_DATA-inline_addr,temp_read);
				concat(CACHE,read_group)(addr+NR_DATA-inline_addr,len+inline_addr-NR_DATA,(uint32_t *)(((uint8_t *)temp_read)+NR_DATA-inline_addr));
				*ans=temp_read[0];
				return true;
 			}
 			else{
				*ans=*(uint32_t *)(CACHE[i][group_no].data+inline_addr);
#ifdef MY_DEBUG
				printf("ans %x %x %x\n",*ans,inline_addr,CACHE[i][group_no].data[5]);
#endif
				*ans=(*ans)&get_1_all(len<<3);
				return true;
			}
 		}
 	}
	return false;
}


bool concat(CACHE,write_group)(hwaddr_t addr,size_t len,uint32_t data){
	uint32_t group_no=get_group_no(addr);
	uint32_t inline_addr=get_inline_addr(addr);
	uint32_t extra_addr=get_extra_addr(addr);
	
	int i;
 	for(i=0;i<SZ_GROUP;i++){
 		if(CACHE[i][group_no].addr_extra==extra_addr&&CACHE[i][group_no].volid_bit==1){
			if(NR_DATA-inline_addr<len){
				uint32_t temp_read;
				concat(CACHE,read_group)(addr+len-1,4,&temp_read);
				concat(CACHE,write_group)(addr,NR_DATA-inline_addr,data);
				concat(CACHE,write_group)(addr+NR_DATA-inline_addr,len+inline_addr-NR_DATA,*(uint32_t *)(((uint8_t *)(&data))+NR_DATA-inline_addr));
 			}
 			else{
				if(len==1)
					unalign_rw(CACHE[i][group_no].data+inline_addr,1)=data;
				else if(len==2)
					unalign_rw(CACHE[i][group_no].data+inline_addr,2)=data;
				else if(len==3)
					unalign_rw(CACHE[i][group_no].data+inline_addr,3)=data;
				else if(len==4)
					unalign_rw(CACHE[i][group_no].data+inline_addr,4)=data;
				CACHE[i][group_no].dirty_bit=1;

			}

#if	WRITE_BACK == 0

#ifdef CACHE_1 

			cache_2_write(addr,len,data);
#endif

#ifdef CACHE_2
			
			dram_write(addr,len,data);
#endif

#endif
			return true;
		}
	}
	return false;
}


void concat(CACHE,add_group)(hwaddr_t addr){
	uint32_t group_no=get_group_no(addr);
	uint32_t inline_addr=get_inline_addr(addr);
	uint32_t extra_addr=get_extra_addr(addr);
	
	int i;
	for(i=0;i<SZ_GROUP;i++){
		if(CACHE[i][group_no].volid_bit==0){
			uint32_t transfer_data;
			uint32_t addr_start=addr-inline_addr;
#ifdef MY_DEBUG
			printf("add group %x %x %x\n",addr,addr_start,group_no);
#endif
			int j;
			for(j=0;j<NR_DATA;j+=4){
#ifdef CACHE_1
				transfer_data=cache_2_read(addr_start+j,4);				
#endif

#ifdef CACHE_2
				transfer_data=dram_read(addr_start+j,4);
#endif
#ifdef MY_DEBUG
				printf("add x %x\n",transfer_data);
#endif
				*((uint32_t *)(CACHE[i][group_no].data+j))=transfer_data;
			}

			CACHE[i][group_no].volid_bit=1;
			CACHE[i][group_no].dirty_bit=0;
			CACHE[i][group_no].addr_extra=extra_addr;

			return ;
		}
	}

	uint32_t rand_i=rand()%SZ_GROUP;
	uint32_t addr_start=addr-inline_addr;
#if WRITE_BACK == 1
	if(CACHE[rand_i][group_no].dirty_bit==1){
		int j;
		for(j=0;j<NR_DATA;j+=4){
#ifdef CACHE_1
			cache_2_write(addr_start+j,4,*((uint32_t *)(CACHE[rand_i][group_no].data+j)));
#endif	

#ifdef CACHE_2
			dram_write(addr_start+j,4,*((uint32_t *)(CACHE[rand_i][group_no].data+j)));
#endif
		}
	}
#endif
	
	uint32_t transfer_data;
	int j;
	for(j=0;j<NR_DATA;j+=4){
#ifdef CACHE_1
		transfer_data=cache_2_read(addr_start+j,4);		
#endif

#ifdef CACHE_2
		transfer_data=dram_read(addr_start+j,4);
#endif

		*((uint32_t *)(CACHE[rand_i][group_no].data+j))=transfer_data;
	}
	CACHE[rand_i][group_no].volid_bit=1;
	CACHE[rand_i][group_no].dirty_bit=0;
	CACHE[rand_i][group_no].addr_extra=extra_addr;
}

uint32_t concat(CACHE,_read)(hwaddr_t addr,size_t len){
	uint32_t ans=0;
#ifdef MY_DEBUG
	printf("in %x %x\n",addr,len);
#endif
	if(concat(CACHE,read_group)(addr,len,&ans)==false){
#ifdef MY_DEBUG
	printf("read false true\n");
#endif
		concat(CACHE,add_group)(addr);
#ifdef MY_DEBUG
		printf("add done true\n");
#endif
		concat(CACHE,read_group)(addr,len,&ans);
 	}
	return ans;
}

void concat(CACHE,_write)(hwaddr_t addr,size_t len, uint32_t data){
	if(concat(CACHE,write_group)(addr,len,data)==false){
#if WRITE_ALLOCATE == 1
		concat(CACHE,add_group)(addr);
		concat(CACHE,write_group)(addr,len,data);

#elif WRITE_ALLOCATE == 0

	#ifdef CACHE_1
		cache_2_write(addr,len,data);
	#endif

	#ifdef CACHE_2
		dram_write(addr,len,data);
	#endif

#endif
	}
}
