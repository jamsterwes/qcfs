#include "chunk.hpp"
#include "root_chunk.hpp"
#include "bt_chunk.hpp"
#include "ft_chunk.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <iostream>
#include "QCFS.hpp"

int main() {

	std::string command = "";
	std::cout << "# Type write or read." << std::endl;
	std::cout << "> ";
	std::cin >> command;
	std::cin.get();

	if (command == "write") {
		QCFS test = QCFS("Test Volume", FOUR_KB, 64);

		ft_entry* test_file = palloc(ft_entry);
		ft_entry* next = &test.ft()->entries[test.ft()->entry_count++];
		uint64_t data_addr = test.info()->block_alloc_count++;
		test_file->ft_id = (byte)ft_entry_id::FILE;
		ft_file_data* fd = &((&test_file->ent_data)->file);
		fd->file_size = 10;
		strcpy_s(fd->name, "test.txt");
		fd->parent = 0;
		fd->first_data_block = data_addr;
		memcpy_s((void*)next, sizeof(ft_entry), (void*)test_file, sizeof(ft_entry));

		memset((void*)test.chunks[data_addr].data, 0x0, DATA_MINUS_HEAD);
		const char* test_text = "Hello World!";
		memcpy_s((void*)test.chunks[data_addr].data, sizeof(char)*strlen(test_text), (void*)test_text, sizeof(char) * strlen(test_text));
		test.chunks[data_addr].exp_ptr = 0LL;
		test.chunks[data_addr].id = (byte)chunk_id::BASE_DATA;
		test.set_bid(data_addr, chunk_id::BASE_DATA);

		size_t bytes = test.dump("test0.qcfs");
		std::cout.imbue(std::locale(""));
		std::cout << (bytes / 1024) << "KB written to test0.qcfs" << std::endl;
	}
	else if (command == "read") {
		QCFS test2 = QCFS::from_file("test0.qcfs");

		uint32_t block_size = test2.info()->block_size_in_bytes;
		uint64_t blocks = test2.info()->size_in_blocks;
		uint64_t alloc = test2.info()->block_alloc_count;
		bt_entry* bt_entries = test2.bt()->entries;

		bool running = true;
		std::string command2 = "";
		while (running) {
			std::cout << "# Type binfo to get block info, vinfo to get volume info, or finfo to get file info (type exit to exit)." << std::endl;
			std::cout << "> ";
			std::cin >> command2;
			std::cin.get();
			if (command2 == "binfo") {
				for (int i = 0; i < blocks; i++) {
					printf("Block %d: 0x%02x\n", i, bt_entries[i].block_id);
				}
			}
			else if (command2 == "vinfo") {
				printf("Block Size: %iKB \nVolume Size (in blocks): %" PRIu64 " blocks\nBlocks Occupied: %" PRIu64 " blocks (%0.2f%%)\n%0.2f KB free\n", block_size / 1024, blocks, alloc, (double)alloc / (double)blocks, ((double)blocks * (double)DATA_MINUS_HEAD) / 1024.0);
			}
			else if (command2 == "finfo") {
				// List file table...
				uint64_t entry_count = test2.ft()->entry_count;
				ft_entry e;
				for (uint64_t i = 0; i < entry_count; i++) {
					e = test2.ft_entry(i);
					if (e.ft_id != (byte)ft_entry_id::BLANK) {
						std::string node_name = "<root>";
						ft_file_data* fd = &((&e.ent_data)->file);
						if (test2.ft_entry(fd->parent).ft_id != (byte)ft_entry_id::ROOT_FOLDER) {
							node_name = test2.ft_entry(fd->parent).ent_data.folder.name;
						}
						switch (e.ft_id) {
						case (byte)ft_entry_id::FILE:
							printf("address %" PRIu64 " | file %s | size %0.2f KB | parent name %s\n", i, fd->name, (double)fd->file_size / 1024.0, node_name.c_str());
							break;
						case (byte)ft_entry_id::FOLDER:
							if (test2.ft_entry(fd->parent).ft_id != (byte)ft_entry_id::ROOT_FOLDER) {
								node_name = test2.ft_entry(fd->parent).ent_data.folder.name;
							}
							printf("folder %s | parent name %s\n", fd->name, node_name.c_str());
							break;
						default:
							break;
						}
					}
				}
			}
			else if (command2.substr(0, 4) == "view") {
				uint64_t address = (uint64_t)std::stoull(command2.substr(5));
				uint64_t db_addr = test2.ft_entry(address).ent_data.file.first_data_block;
				char* data = (char*)test2.chunks[db_addr].data;
				printf("contents: %s\n", data);
			}
			else if (command2 == "exit") {
				running = false;
			}
		}
	}

	return 0;
}