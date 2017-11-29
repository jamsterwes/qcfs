#pragma once
#include "QCFSStream.h"
#include <vector>

class QCFSRecord
{
public:
	virtual ~QCFSRecord() {}
	/* Called by file writer on a `QCFSRecord`: converts that record into a stream of `QCFSBlock`s */
	virtual QCFSStream serialize() abstract = 0;
};

