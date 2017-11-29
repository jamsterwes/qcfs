#pragma once
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define GB * 1024 * 1024 * 1024
#define MB * 1024 * 1024
#define KB * 1024

/* Allocates single pointer to element of type X */
#define palloc(X) (X*)malloc(sizeof(X))

/* Allocates pointer to array of elements of type X with length Y */
#define pallocs(X, Y) (X*)malloc(sizeof(X) * Y)

typedef uint8_t byte;
typedef uint64_t chunk_ptr;

enum class QCFSBlockSizes : uint32_t {
	FOUR_KB = 4 * 1024,
	SIXTY_FOUR_KB = 64 * 1024,
	DEPRECATED_TWO_MB = 2 * 1024 * 1024
};
const uint32_t BLOCK_SIZE = (uint32_t)QCFSBlockSizes::FOUR_KB;