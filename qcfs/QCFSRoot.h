#pragma once
#include "QCFSRecord.h"

class QCFSRoot :
	public QCFSRecord
{
public:
	QCFSRoot();
	// QCFSRoot(QCFSStream root_stream);
	QCFSRoot(uint32_t block_size, uint64_t block_amount, uint64_t blocks_alloc, char* volume_name);
	~QCFSRoot();

	/* Attributes */
	uint32_t block_size;
	uint64_t block_amount;
	uint64_t blocks_alloc;
	char volume_name[128];

	QCFSStream serialize() override;
};

const uint32_t ROOT_PADDING = DATA_SIZE - sizeof(uint32_t) - sizeof(uint64_t) - sizeof(uint64_t) - sizeof(block_ptr) - sizeof(block_ptr) - (sizeof(char) * 128);

#pragma pack(push, 1)
struct QCFSRootData {
	uint32_t block_size;
	uint64_t block_amount;
	uint64_t blocks_alloc;
	block_ptr block_table;
	block_ptr file_table;
	char volume_name[128];
	byte padding[ROOT_PADDING];
};
#pragma pack(pop)