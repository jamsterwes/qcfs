#include "chunk.h"
#include "root_chunk.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <iostream>
#include "QCFS.h"

int main() {
	std::cout << "Type write to write the test file or read to read it back" << std::endl;
	std::cout << "> ";

	std::string command = "";
	std::cin >> command;
	std::cin.get();

	if (command == "write") {
		QCFS test = QCFS("Test Volume", TWO_MB, 8);

		root_data* test_info = palloc(root_data);
		test.info(test_info);

		FILE* output;
		fopen_s(&output, "test0.qcfs", "wb");
		size_t bytes = test.dump(output);
		fclose(output);

		std::cout.imbue(std::locale(""));
		std::cout << (bytes / 1024) << "KB written to test0.qcfs" << std::endl;
		std::cin.get();
	} else if (command == "read") {
		chunk* toor = palloc(chunk);
		chunk* chunks;

		FILE* input;
		fopen_s(&input, "test0.qcfs", "rb");
		fread((void*)toor, TWO_MB, 1, input);
		root_data* newinfo = (root_data*)toor->data;
		uint64_t blocks = newinfo->size_in_blocks;
		chunks = pallocs(chunk, blocks);
		fseek(input, 0, 0);
		fread((void*)chunks, TWO_MB, blocks, input);
		fclose(input);


		printf("Block Size: %iKB \nVolume Size (in blocks): %" PRIu64 " blocks\nBlocks Occupied: %" PRIu64 " blocks (%0.2f%%)\n", newinfo->block_size_in_bytes / 1024, newinfo->size_in_blocks, newinfo->block_alloc_count, (double)newinfo->block_alloc_count / (double)newinfo->size_in_blocks);

		std::cin.get();
	}

	return 0;
}