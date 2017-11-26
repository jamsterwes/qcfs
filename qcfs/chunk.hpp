#pragma once
#include <stdint.h>

#define palloc(X) (X*)malloc(sizeof(X))
#define pallocs(X, Y) (X*)malloc(sizeof(X) * Y)

typedef uint8_t byte;
typedef uint64_t chunk_ptr;

const uint32_t TWO_MB = 2 * 1024 * 1024;
const uint32_t FOUR_KB = 4 * 1024;
const uint32_t BLOCK_SIZE = FOUR_KB;
const uint32_t DATA_MINUS_HEAD = FOUR_KB - sizeof(byte) - sizeof(chunk_ptr);


enum class chunk_id : byte {
	BASE_ROOT = 0x00,
	BASE_FOLDER = 0x01,
	BASE_FILE = 0x02,
	BASE_BT = 0x03,
	BASE_DATA = 0x04,
	EXPANSION_ROOT = 0xF0,
	EXPANSION_FT = 0xF1,
	EXPANSION_BT = 0xF2,
	EXPANSION_DATA = 0xF3,
	FREE = 0x10
};

#pragma pack(push, 1)
struct chunk_2MB {
	byte id;
	byte data[DATA_MINUS_HEAD];
	chunk_ptr exp_ptr;
};
#pragma pack(pop)

typedef chunk_2MB chunk;