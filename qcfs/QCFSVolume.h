#pragma once
#include "QCFSRoot.h"
#include "QCFSBlockTable.h"

class QCFSVolume
{
public:
	QCFSRoot root;
	QCFSBlockTable bt;
	// QCFSFileTable ft;

	QCFSVolume();
	QCFSVolume(std::string volume_name, uint64_t byte_size);
	~QCFSVolume();

	// void write_to_disk(std::string file_path);
	// void read_from_disk(std::string file_path);
};

