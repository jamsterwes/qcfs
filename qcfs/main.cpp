#include "chunk.hpp"
#include "root_chunk.hpp"
#include "bt_chunk.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <iostream>
#include "QCFS.hpp"

void gracefully_exit() {
	std::cout << std::endl << "Press enter to exit." << std::endl;
	std::cin.get();
}

int main() {
	std::cout << "Type write to write the test file or read to read it back" << std::endl;
	std::cout << "> ";

	std::string command = "";
	std::cin >> command;
	std::cin.get();

	if (command == "write") {
		QCFS test = QCFS("Test Volume", FOUR_KB, 64);

		root_data* test_info = test.info();

		size_t bytes = test.dump("test0.qcfs");

		std::cout.imbue(std::locale(""));
		std::cout << (bytes / 1024) << "KB written to test0.qcfs" << std::endl;

		gracefully_exit();
	} else if (command == "read") {

		QCFS test2 = QCFS::from_file("test0.qcfs");

		uint32_t block_size = test2.info()->block_size_in_bytes;
		uint64_t blocks = test2.info()->size_in_blocks;
		uint64_t alloc = test2.info()->block_alloc_count;
		bt_entry* bt_entries = test2.bt()->entries;

		printf("Block Size: %iKB \nVolume Size (in blocks): %" PRIu64 " blocks\nBlocks Occupied: %" PRIu64 " blocks (%0.2f%%)\n", block_size / 1024, blocks, alloc, (double)alloc / (double)blocks);

		for (int i = 0; i < blocks; i++) {
			printf("Block %d: 0x%02x\n", i, bt_entries[i]);
		}

		gracefully_exit();
	}

	return 0;
}