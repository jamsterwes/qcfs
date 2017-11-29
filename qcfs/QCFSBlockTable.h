#pragma once
#include "QCFSRecord.h"

class QCFSBlockTable :
	public QCFSRecord
{
public:
	QCFSBlockTable();
	// QCFSBlockTable(QCFSStream bt_stream);
	QCFSBlockTable(uint64_t block_count);
	QCFSBlockTable(block_id* block_ids, uint64_t block_count);
	~QCFSBlockTable();

	uint64_t count;

	void set(block_id id, block_ptr ptr);

	QCFSStream serialize() override;
private:
	block_id* ids;  // Pointer array of types
};

#pragma pack(push, 1)
struct QCFSBlockEntry {
	byte block_type;  // type `block_id` (type of entry's block)
};
#pragma pack(pop)

const uint32_t BT_BLOCKAMT = DATA_SIZE / sizeof(QCFSBlockEntry);
const uint32_t BT_PADDING = DATA_SIZE % sizeof(QCFSBlockEntry);

#pragma pack(push, 1)
struct QCFSBlockData {
	QCFSBlockEntry entries[BT_BLOCKAMT];  // array of `QCFSBlockEntry`s for listing the type of the block with the corresponding index
};
#pragma pack(pop)