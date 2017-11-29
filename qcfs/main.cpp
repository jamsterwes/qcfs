#include <iostream>
#include <inttypes.h>
#include "QCFSStream.h"
#include "QCFSVolume.h"

int main() {
	QCFSBlock* blocks = pallocs(QCFSBlock, (256 MB) / (BLOCK_SIZE));
	uint64_t next_available = 0;
	QCFSVolume vol = QCFSVolume("Hello World", 256 MB);
	// Write Root Table Block
	vol.root.serialize().for_each([&](QCFSBlock* current) {
		if (current->id == (byte)block_id::EXPANSION_ROOT) {
			blocks[next_available - 1].expansion = next_available;
		}
		vol.bt.set((block_id)current->id, next_available);
		memcpy_s((void*)&blocks[next_available++], BLOCK_SIZE, (void*)current, BLOCK_SIZE);
		printf("Block ID: 0x%02x (%s)\nBlock Expansion %" PRIu64 "\n", current->id, block_id_to_string((block_id)current->id).c_str(), current->expansion);
	});
	// Write Block Table Blocks
	vol.bt.serialize().for_each([&](QCFSBlock* current) {
		if (current->id == (byte)block_id::EXPANSION_BT) {
			blocks[next_available - 1].expansion = next_available;
		}
		for (uint64_t btct = 0; btct < BT_BLOCKAMT; btct++) {
			printf("Block ID: 0x%02x (%s)\n", ((QCFSBlockData*)current->data)->entries[btct], block_id_to_string((block_id)((QCFSBlockData*)current->data)->entries[btct].block_type).c_str());
		}
		memcpy_s((void*)&blocks[next_available++], BLOCK_SIZE, (void*)current, BLOCK_SIZE);
		printf("Block ID: 0x%02x (%s)\nBlock Expansion %" PRIu64 "\n", current->id, block_id_to_string((block_id)current->id).c_str(), current->expansion);
	});

	std::cin.get();

	return 0;
}