#pragma once
#include "chunk.hpp"
const uint32_t ROOT_PADDING = DATA_MINUS_HEAD - sizeof(uint32_t) - sizeof(uint64_t) - sizeof(uint64_t) - sizeof(chunk_ptr) - sizeof(chunk_ptr) - (sizeof(char) * 128);

#pragma pack(push, 1)
struct root_data {
	uint32_t block_size_in_bytes;
	uint64_t size_in_blocks;
	uint64_t block_alloc_count;
	chunk_ptr block_table_ptr;
	chunk_ptr file_table_ptr;
	char volume_name[128];
	byte _padding[ROOT_PADDING];
};
#pragma pack(pop)

void root_chunk(root_data* rinfo, chunk* c);