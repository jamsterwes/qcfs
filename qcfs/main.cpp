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
#include <queue>
#include <Windows.h>

void display_help() {
	std::cout << "commands:" << std::endl << "--help" << std::endl << "* Displays this help screen." << std::endl << "--create [file name] [volume name] [size in bytes] [folder path]" << std::endl << "* Creates a new qcfs file." << std::endl << "--extract [file name] [output path]" << std::endl  << "* Extracts the qcfs file into the output path." << std::endl << "--console" << std::endl << "* Opens debug console for developers." << std::endl;
}

void load_root_folder(std::string base_path, QCFS* volume) {
	HANDLE hFind;
	WIN32_FIND_DATA data;

	hFind = FindFirstFile((base_path + "\\*.*").c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			std::string s = std::string(data.cFileName);
		} while (FindNextFile(hFind, &data));
	}
}

int main(int argc, char** argv) {
	if (argc >= 2) {
		if (std::string(argv[1]) == "--help") {
			display_help();
		}
		else if (argc >= 6 && std::string(argv[1]) == "--create") {
			QCFS volume = QCFS(argv[3], FOUR_KB, std::stoull(std::string(argv[4])) / FOUR_KB + (std::stoull(std::string(argv[4])) % FOUR_KB > 0 ? 1 : 0));
			std::string base_path = std::string(argv[5]);
			load_root_folder(base_path, &volume);
			/* for (int i = 5; i < argc; i++) {
				const char* fn = argv[i];
				if (!is_folder) {
					FILE* infile;
					fopen_s(&infile, fn, "rb");
					size_t bytes;
					fseek(infile, 0, SEEK_END);
					bytes = ftell(infile);
					rewind(infile);
					byte* data = pallocs(byte, bytes);
					fread((void*)data, sizeof(byte), bytes, infile);
					fclose(infile);

					volume.add_file((char*)fn, data, bytes);
				}
			} */

			volume.dump(std::string(argv[2]));
		}
		else if (argc >= 4 && std::string(argv[1]) == "--extract") {
			QCFS volume = QCFS::from_file(argv[2]);
			std::string output_path = argv[3];
			if (output_path[output_path.length() - 1] == '/') {
				output_path = output_path.substr(0, output_path.length() - 1);
			}
			uint64_t entry_count = volume.ft()->entry_count;
			for (int e = 0; e < entry_count; e++) {
				ft_entry* entry = &volume.ft()->entries[e];
				if (entry->ft_id == (byte)ft_entry_id::FILE) {
					FILE* outfile;
					fopen_s(&outfile, (output_path + "/" + std::string(entry->ent_data.file.name)).c_str(), "wb");
					uint64_t block_count = entry->ent_data.file.file_size / DATA_MINUS_HEAD;
					if (entry->ent_data.file.file_size % DATA_MINUS_HEAD > 0) {
						block_count++;
					}
					uint64_t data_addr = entry->ent_data.file.first_data_block;
					byte* data;
					uint64_t ct = 0;
					uint64_t curr_size;
					while (data_addr != 0) {
						if (ct == block_count - 1) {
							curr_size = entry->ent_data.file.file_size % DATA_MINUS_HEAD;
						}
						else {
							curr_size = DATA_MINUS_HEAD;
						}
						data = pallocs(byte, curr_size);
						memcpy_s((void*)data, curr_size, (void*)&volume.chunks[data_addr].data, curr_size);
						fwrite((void*)data, curr_size, 1, outfile);
						free((void*)data);
						data_addr = volume.chunks[data_addr].exp_ptr;
						ct++;
					}
					fclose(outfile);
				}
			}
		}
		else if (std::string(argv[1]) == "--console") {
			std::string command = "";
			std::cout << "# Type write or read." << std::endl;
			std::cout << "> ";
			std::cin >> command;
			std::cin.get();

			if (command.substr(0,4) == "read") {
				QCFS test2 = QCFS::from_file(command.substr(5));

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
						printf("Block Size: %iKB \nVolume Size (in blocks): %" PRIu64 " blocks\nBlocks Occupied: %" PRIu64 " blocks (%0.2f%%)\n%0.2fKB/%0.2fKB free\n", block_size / 1024, blocks, alloc, ((double)alloc / (double)blocks) * 100.0, ((double)(blocks - alloc) * (double)DATA_MINUS_HEAD) / 1024.0, ((double)blocks * (double)DATA_MINUS_HEAD) / 1024.0);
					}
					else if (command2 == "finfo") {
						// List file table...
						uint64_t entry_count = test2.ft()->entry_count;
						ft_entry e;
						for (uint64_t i = 0; i < entry_count; i++) {
							e = test2.ft_entryi(i);
							if (e.ft_id != (byte)ft_entry_id::BLANK) {
								std::string node_name = "<root>";
								ft_file_data* fd = &((&e.ent_data)->file);
								if (test2.ft_entryi(fd->parent).ft_id != (byte)ft_entry_id::ROOT_FOLDER) {
									node_name = test2.ft_entryi(fd->parent).ent_data.folder.name;
								}
								switch (e.ft_id) {
								case (byte)ft_entry_id::FILE:
									printf("address %" PRIu64 " | file %s | size %0.2f KB | parent name %s\n", i, fd->name, (double)fd->file_size / 1024.0, node_name.c_str());
									break;
								case (byte)ft_entry_id::FOLDER:
									if (test2.ft_entryi(fd->parent).ft_id != (byte)ft_entry_id::ROOT_FOLDER) {
										node_name = test2.ft_entryi(fd->parent).ent_data.folder.name;
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
						uint64_t db_addr = test2.ft_entryi(address).ent_data.file.first_data_block;
						char* data = (char*)test2.chunks[db_addr].data;
						printf("contents: %s\n", data);
					}
					else if (command2 == "exit") {
						running = false;
					}
				}
			}
		}
		else {
			display_help();
		}
	}
	else {
		display_help();
	}
	return 0;
}