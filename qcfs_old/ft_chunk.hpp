#pragma once
#include "chunk.hpp"
#include "ft_entry.hpp"

const uint32_t FT_ENTRYAMT = (DATA_MINUS_HEAD - sizeof(uint64_t)) / sizeof(ft_entry);
const uint32_t FT_PADDING = (DATA_MINUS_HEAD - sizeof(uint64_t)) % sizeof(ft_entry);

#pragma pack(push, 1)
struct ft_data {
	uint64_t entry_count;
	ft_entry entries[FT_ENTRYAMT];
	byte __padding__[FT_PADDING];
};
#pragma pack(pop)

void ft_chunk(chunk* c);