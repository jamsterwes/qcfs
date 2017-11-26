#include "QCFS.hpp"

QCFS::QCFS(chunk* ichunks) {
	root_data* rootd = ((root_data*)((&ichunks[0])->data));
	bs = rootd->block_size_in_bytes;
	bc = rootd->size_in_blocks;
	chunks = pallocs(chunk, bs);
	memcpy_s((void*)chunks, bc * bs, (void*)ichunks, bs * bc);
}

QCFS::QCFS(std::string volume_name, uint32_t block_size, uint64_t blocks) : bc(blocks), bs(block_size)
{
	chunks = pallocs(chunk, blocks);
	memset((byte*)chunks, 0, sizeof(chunk) * blocks);

	// Init Root
	chunk* root = init_root(volume_name, block_size, blocks);
	set_chunk(0, root);

	// Init Block Table
	chunk* bt = init_bt();
	set_chunk(1, bt);
	info()->block_table_ptr = 1;

	// Populate Block Table
	set_bid(0, chunk_id::BASE_ROOT);
	set_bid(1, chunk_id::BASE_BT);
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
	((root_data*)((&chunks[0])->data))->block_alloc_count++;
	bt_chunk(bt);
	return bt;
}

void QCFS::set_bid(uint64_t block, chunk_id bid) {
	(&((bt_data*)((&chunks[1])->data))->entries[block])->block_id = (byte)bid;
}

void QCFS::set_chunk(uint64_t pos, chunk* c) {
	chunks[pos] = *c;
}

root_data* QCFS::info() {
	return (root_data*)((&chunks[0])->data);
}

bt_data* QCFS::bt() {
	return (bt_data*)((&chunks[1])->data);
}

QCFS QCFS::from_file(std::string filename)
{
	chunk* toor = palloc(chunk);
	chunk* _chunks;

	FILE* input;
	fopen_s(&input, "test0.qcfs", "rb");
	fread((void*)toor, BLOCK_SIZE, 1, input);
	root_data* newinfo = (root_data*)toor->data;
	uint64_t blocks = newinfo->size_in_blocks;
	_chunks = pallocs(chunk, blocks);
	fseek(input, 0, 0);
	fread((void*)_chunks, FOUR_KB, blocks, input);
	fclose(input);

	return QCFS(_chunks);
}

size_t QCFS::dump(std::string filename) {
	FILE *fp;
	fopen_s(&fp, filename.c_str(), "wb");
	size_t output = bs * fwrite(chunks, bs, bc, fp);
	fclose(fp);
	return output;
}