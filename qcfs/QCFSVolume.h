#pragma once
#include "QCFSRoot.h"

class QCFSVolume
{
public:
	QCFSRoot root;
	// QCFSBlockTable bt;
	// QCFSFileTable ft;

	QCFSVolume();
	~QCFSVolume();
};

