#pragma once
#include "chunk.hpp"

typedef chunk_ptr ft_ptr;

enum class ft_entry_id : byte {
	ROOT_FOLDER = 0x00,
	FILE = 0x01,
	FOLDER = 0x02,
	BLANK = 0xFF
};

#pragma pack(push, 1)
struct ft_file_data {
	ft_ptr parent;
	char name[128];
	uint64_t file_size;
	chunk_ptr first_data_block;
};

struct ft_folder_data {
	ft_ptr parent;
	char name[128];
	uint64_t __padding0__;
	chunk_ptr __padding1__;
};

struct ft_root_data {
	ft_ptr __padding0__;
	char __padding1__[128];
	uint64_t __padding2__;
	chunk_ptr __padding3__;
};

union ft_entry_data {
	ft_file_data file;
	ft_folder_data folder;
	ft_root_data __root;
};

struct ft_entry {
	byte ft_id;
	ft_entry_data ent_data;
};
#pragma pack(pop)