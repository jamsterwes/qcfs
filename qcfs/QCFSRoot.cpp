#include "QCFSRoot.h"
#include <Windows.h>

QCFSRoot::QCFSRoot(): QCFSRecord()
{
}


QCFSRoot::QCFSRoot(uint32_t _block_size, uint64_t _block_amount, uint64_t _blocks_alloc, char* _volume_name) : block_size(_block_size), block_amount(_block_amount), blocks_alloc(_blocks_alloc)
{
	ZeroMemory(volume_name, 128 * sizeof(char));
	strcpy_s(volume_name, _volume_name);
}

QCFSRoot::~QCFSRoot()
{
}

QCFSStream QCFSRoot::serialize()
{
	QCFSStream s;
	QCFSBlock* b = palloc(QCFSBlock);
	QCFSRootData* root_data = palloc(QCFSRootData);
	root_data->blocks_alloc = blocks_alloc;
	root_data->block_size = block_size;
	root_data->block_amount = block_amount;
	root_data->file_table = 0LL;
	root_data->block_table = 0LL;
	ZeroMemory(root_data->padding, ROOT_PADDING);
	strcpy_s(root_data->volume_name, volume_name);
	memcpy_s((void*)b->data, DATA_SIZE, (void*)root_data, DATA_SIZE);
	b->id = (byte)block_id::BASE_ROOT;
	b->expansion = 0LL;
	s.insert(b);
	return s;
}
