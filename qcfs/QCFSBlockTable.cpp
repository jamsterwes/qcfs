#include "QCFSBlockTable.h"
#include <inttypes.h>
#include <Windows.h>

QCFSBlockTable::QCFSBlockTable()
{
}

QCFSBlockTable::QCFSBlockTable(uint64_t block_count) : count(block_count)
{
	ids = pallocs(block_id, block_count);
	ZeroMemory(ids, block_count);
}

QCFSBlockTable::QCFSBlockTable(block_id* block_ids, uint64_t block_count) : count(block_count)
{
	ids = pallocs(block_id, block_count);
	memcpy_s((void*)ids, block_count * sizeof(block_id), (void*)block_ids, block_count * sizeof(block_id));
}

QCFSBlockTable::~QCFSBlockTable()
{
}

void QCFSBlockTable::set(block_id id, block_ptr ptr)
{
	ids[ptr] = id;
}

QCFSStream QCFSBlockTable::serialize()
{
	QCFSStream s;
	uint64_t current_block = 0;
	uint64_t block_amount = (uint64_t)((count / BT_BLOCKAMT) + (uint64_t)(count % BT_BLOCKAMT > 0));
	QCFSBlock* b = palloc(QCFSBlock);
	QCFSBlockData* bd = palloc(QCFSBlockData);
	for (uint64_t c = 0; c < block_amount; c++) {
		b->expansion = 0LL;
		if (c == 0) {
			b->id = (byte)block_id::BASE_BT;
		}
		else {
			b->id = (byte)block_id::EXPANSION_BT;
		}
		uint64_t blocks_in_block;
		if (c < block_amount - 1) {
			blocks_in_block = BT_BLOCKAMT;
		}
		else {
			blocks_in_block = count % BT_BLOCKAMT;
		}
		QCFSBlockEntry* e = palloc(QCFSBlockEntry);
		for (uint64_t bdi = 0; bdi < blocks_in_block; bdi++) {
			e->block_type = (byte)ids[c * blocks_in_block + bdi];
			memcpy_s((void*)&bd->entries[bdi], sizeof(QCFSBlockEntry), (void*)e, sizeof(QCFSBlockEntry));
		}
		memcpy_s((void*)b->data, DATA_SIZE, (void*)bd, DATA_SIZE);
		s.insert(b);
	}
	return s;
}
