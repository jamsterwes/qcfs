#pragma once
#include "chunk.hpp"
#include "root_chunk.hpp"
#include "bt_chunk.hpp"
#include "ft_chunk.hpp"
#include <string>

class QCFS
{
public:
	chunk* chunks;

	QCFS(chunk* chunks);
	QCFS(std::string volume_name, uint32_t block_size, uint64_t blocks);
	~QCFS();

	size_t dump(std::string filename);
	root_data* info();
	bt_data* bt();
	ft_data* ft();
	ft_entry* root_folder();
	ft_entry ft_entryi(ft_ptr ptr);

	chunk* make_data_block(uint64_t offset, byte * data, uint64_t size);

	void add_file(char* filename, byte* data, uint64_t size);

	void set_bid(uint64_t block, chunk_id bid); // DONT LEAVE AS PUBLIC
	static QCFS from_file(std::string filename);

private:
	chunk* init_root(std::string volume_name, uint32_t block_size, uint64_t blocks);
	chunk* init_bt();
	chunk* init_ft();
	void set_chunk(uint64_t pos, chunk* c);
	// void set_bid(uint64_t block, chunk_id bid);

	uint64_t bc;
	uint32_t bs;
};

