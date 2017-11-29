#pragma once
#include "QCFSUtil.h"

typedef uint64_t block_ptr;

// Size of data after headers
const uint64_t DATA_SIZE = BLOCK_SIZE - sizeof(byte) - sizeof(block_ptr);

/* Stores type of block
- High Values: 0 (Base), F (Expansion), 1 (Free);
- Low Values: 0 (Root), 1 (Block Table), 2 (File Table), 3 (File Data) */
enum class block_id : byte {
	BASE_ROOT = 0x00,
	BASE_BT = 0x01,
	BASE_FT = 0x02,
	BASE_DATA = 0x03,
	EXPANSION_ROOT = 0xF0,
	EXPANSION_BT = 0xF1,
	EXPANSION_FT = 0xF2,
	EXPANSION_DATA = 0xF3,
	FREE = 0x10
};

std::string block_id_to_string(block_id id);

#pragma pack(push, 1)
// Pure byte/integer representation of the data in a block (1-byte aligned)
struct QCFSBlock {
	byte id;  // type of block: (see `block_id`)
	byte data[DATA_SIZE];  // Data payload of block: many different uses
	block_ptr expansion;  // Used if data required exceeds data block, points to next data block filled (or if == 0, then no expansion is used)
};
#pragma pack(pop)