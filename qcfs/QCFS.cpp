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

	// Init File Table
	chunk* ft = init_ft();
	set_chunk(2, ft);
	info()->file_table_ptr = 2;

	// Populate Block Table
	set_bid(0, chunk_id::BASE_ROOT);
	set_bid(1, chunk_id::BASE_BT);
	set_bid(2, chunk_id::BASE_FT);
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
	info()->block_alloc_count++;
	bt_chunk(bt);
	return bt;
}

chunk* QCFS::init_ft() {
	chunk* ft = palloc(chunk);
	info()->block_alloc_count++;
	ft_chunk(ft);
	return ft;
}

void QCFS::set_bid(uint64_t block, chunk_id bid) {
	(&(bt()->entries[block]))->block_id = (byte)bid;
}

void QCFS::set_chunk(uint64_t pos, chunk* c) {
	chunks[pos] = *c;
}

root_data* QCFS::info() {
	return (root_data*)((&chunks[0])->data);
}

bt_data* QCFS::bt() {
	return (bt_data*)((&chunks[info()->block_table_ptr])->data);
}

ft_data* QCFS::ft() {
	return (ft_data*)((&chunks[info()->file_table_ptr])->data);
}

ft_entry* QCFS::root_folder() {
	return &(ft()->entries[0]);
}

ft_entry QCFS::ft_entryi(ft_ptr ptr)
{
	return ft()->entries[ptr];
}

chunk* QCFS::make_data_block(uint64_t offset, byte* data, uint64_t size) {
	chunk* db = palloc(chunk);
	memset((void*)db->data, 0x0, DATA_MINUS_HEAD);
	memcpy_s((void*)db->data, size, (void*)(data + offset), size);
	return db;
}

void QCFS::add_file(char* filename, byte* data, uint64_t size)
{
	ft_entry* test_file = palloc(ft_entry);
	ft_entry* next = &ft()->entries[ft()->entry_count++];
	test_file->ft_id = (byte)ft_entry_id::FILE;
	ft_file_data* fd = &((&test_file->ent_data)->file);
	fd->file_size = size;
	strcpy_s(fd->name, filename);
	fd->parent = 0;
	uint64_t data_addr = info()->block_alloc_count++;
	uint64_t last_data_addr = 0LL;
	fd->first_data_block = data_addr;
	memcpy_s((void*)next, sizeof(ft_entry), (void*)test_file, sizeof(ft_entry));
	
	uint64_t block_count = size / DATA_MINUS_HEAD;
	if (size % DATA_MINUS_HEAD > 0) {
		block_count++;
	}
	for (int b = 0; b < block_count; b++) {
		uint64_t pad = (size - (b * DATA_MINUS_HEAD)) % DATA_MINUS_HEAD;
		uint64_t curr_size = DATA_MINUS_HEAD;
		if (b == block_count - 1) {
			curr_size = pad;
		}
		chunk* cdb = make_data_block(b * DATA_MINUS_HEAD, data, curr_size);
		memcpy_s((void*)&chunks[data_addr], sizeof(chunk), (void*)cdb, sizeof(chunk));

		chunks[data_addr].id = (byte)chunk_id::BASE_DATA;
		set_bid(data_addr, chunk_id::BASE_DATA);
		last_data_addr = data_addr;
		data_addr = info()->block_alloc_count++;
		if (b < block_count - 1) {
			chunks[last_data_addr].exp_ptr = data_addr;
		}
		else {
			chunks[last_data_addr].exp_ptr = 0LL;
		}
	}
	info()->block_alloc_count--;
}

QCFS QCFS::from_file(std::string filename)
{
	chunk* toor = palloc(chunk);
	chunk* _chunks;

	FILE* input;
	fopen_s(&input, filename.c_str(), "rb");
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