typedef struct SegmentDescriptor {
	uint32_t limit_15_0          : 16;
	uint32_t base_15_0           : 16;
	uint32_t base_23_16          : 8;
	uint32_t type                : 4;
	uint32_t segment_type        : 1;
	uint32_t privilege_level     : 2;
	uint32_t present             : 1;
	uint32_t limit_19_16         : 4;
	uint32_t soft_use            : 1;
	uint32_t operation_size      : 1;
	uint32_t pad0                : 1;
	uint32_t granularity         : 1;
	uint32_t base_31_24          : 8;
} SegDesc;

typedef union{
	struct{
		uint32_t base_15_0           : 16;
		uint32_t base_23_16          : 8;
		uint32_t base_31_24          : 8;
	};
	uint32_t data;
}SD_base;

typedef union{
	struct{
		uint32_t limit_15_0          : 16;
		uint32_t limit_19_16         : 4;
	};
	uint32_t data;
}SD_limit;

typedef union PageDirectoryEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t pad0                : 6;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PDE;

typedef union PageTableEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t dirty               : 1;
		uint32_t pad0                : 1;
		uint32_t global              : 1;
		uint32_t pad1                : 3;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PTE;

typedef struct GateDescriptor {
	uint32_t offset_15_0      : 16;
	uint32_t segment          : 16;
	uint32_t pad0             : 8;
	uint32_t type             : 4;
	uint32_t system           : 1;
	uint32_t privilege_level  : 2;
	uint32_t present          : 1;
	uint32_t offset_31_16     : 16;
} GateDesc;

typedef union{
	struct{
		uint16_t t_16_0;
		uint16_t t_16_1;
	};
	uint32_t t_32;
}t_16_16;

typedef union {
	struct{
		uint32_t t_12 :12;
		uint32_t t_20 :20;
	};
	uint32_t val;
}t_12_20;

typedef union{
	struct{
		uint32_t t_12:12;
		uint32_t t_10_l:10;
		uint32_t t_10_h:10;
	};
	uint32_t val;
}t_12_10_10;
