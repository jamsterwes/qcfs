#include "QCFS.hpp"
#include "bt_chunk.hpp"

QCFS::QCFS(std::string volume_name, uint32_t block_size, uint64_t blocks) : bc(blocks), bs(block_size)
{
	chunk* root = init_root(volume_name, block_size, blocks);
	chunk* bt = 

	chunks = pallocs(chunk, blocks);
	memset((byte*)chunks, 0, sizeof(chunk) * blocks);
	set_chunk(0, root);
	set_chunk(1, bt);
}

QCFS::~QCFS()
{
}

chunk* QCFS::init_root(std::string volume_name, uint32_t block_size, uint64_t blocks) {
	root_data* rd = palloc(root_data);
	strcpy_s(rd->volume_name, &volume_name[0]);
	rd->block_alloc_count = 1LL;
	rd->block_size_in_bytes = block_size;
	rd->size_in_blocks = blocks;
	memset(rd->_padding, 0, ROOT_PADDING);
	rd->file_table_ptr = (chunk_ptr)0LL;
	rd->block_table_ptr = (chunk_ptr)0LL;

	chunk* root = palloc(chunk);
	root_chunk(rd, root);
	return root;
}

chunk* QCFS::init_bt() {
	chunk* bt = palloc(chunk);
	bt_chunk(bt);
	return bt;
}

void QCFS::set_chunk(uint64_t pos, chunk* c) {
	chunks[pos] = *c;
}

void QCFS::info(root_data* rd) {
	memcpy_s((void*)rd, sizeof(root_data), (void*)(chunks[0].data), sizeof(root_data));
}

size_t QCFS::dump(FILE* fp) {
	return fwrite(chunks, bs, bc, fp);
}