#include "QCFSVolume.h"


QCFSVolume::QCFSVolume()
{
}

QCFSVolume::QCFSVolume(std::string volume_name, uint64_t byte_size)
{
	uint32_t fourkb_ui64 = (uint32_t)QCFSBlockSizes::FOUR_KB;
	uint64_t block_count = (uint64_t)((byte_size / (uint64_t)fourkb_ui64) + (uint64_t)(byte_size % (uint64_t)fourkb_ui64 > 0));
	root = QCFSRoot(fourkb_ui64, block_count, 1LL, (char*)volume_name.c_str());
	bt = QCFSBlockTable(block_count);
}


QCFSVolume::~QCFSVolume()
{
}
