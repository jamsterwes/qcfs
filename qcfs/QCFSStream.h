#pragma once
#include <functional>
#include "QCFSUtil.h"
#include "QCFSBlock.h"

class QCFSStream
{
public:
	QCFSStream();
	~QCFSStream();

	/* Inserts the element at pointer `rhs` at the end of the `_elements` array */
	void insert(QCFSBlock* rhs);

	/* Pops the element at the front of the `_elements` array */
	void next(QCFSBlock* block);

	/* Returns if the stream is empty */
	bool empty();

	/* Stores the length as a 64-bit unsigned integer */
	uint64_t length;

	/* Takes in a loop function whose parameters are the current block, and two setters to set the file_table id, and the block_table id */
	void for_each(std::function<void(QCFSBlock*)> loopfunc);
private:
	/* Pointer-style array of the blocks to stream */
	QCFSBlock* _elements;

	/* Resizes `_elements` array to size `newLength` */
	void _resize_elements(uint64_t newLength);

	/* Shifts the elements of the `elements` array one position to the left, discarding the first element */
	void _shiftl_elements();
};

