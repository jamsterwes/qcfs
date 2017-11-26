#pragma once
#include "chunk.hpp"
#include "root_chunk.hpp"
#include <string>

class QCFS
{
public:
	chunk* chunks;

	QCFS(std::string volume_name, uint32_t block_size, uint64_t blocks);
	~QCFS();

	size_t dump(FILE* fp);
	void info(root_data* rd);

private:
	chunk* init_root(std::string volume_name, uint32_t block_size, uint64_t blocks);
	chunk* QCFS::init_bt();
	void set_chunk(uint64_t pos, chunk* c);

	uint64_t bc;
	uint32_t bs;
};

