#include "QCFSStream.h"

QCFSStream::QCFSStream() : length(0),  // Start off with the length 0
						_elements(nullptr)  // Start off with an empty array
{ 
}

QCFSStream::~QCFSStream()
{
}

/* Inserts the element at pointer `rhs` at the end of the `_elements` array */
void QCFSStream::insert(QCFSBlock* rhs) {
	_resize_elements(++length);  // Resize the array so that we have an empty space at the end
	memcpy_s((void*)&_elements[length - 1], sizeof(QCFSBlock), (void*)rhs, sizeof(QCFSBlock));  // Place the value at the pointer into the new space created
}

/* Returns if the stream is empty */
bool QCFSStream::empty() {
	return _elements == nullptr;  // Check to see if the pointer is set to `nullptr` (meaning the array is empty)
}

/* Takes in a loop function whose parameters are the current block, and two setters to set the file_table id, and the block_table id */
void QCFSStream::for_each(std::function<void(QCFSBlock*)> loopfunc)
{
	QCFSBlock* current = palloc(QCFSBlock);
	do {
		this->next(current);
		loopfunc(current);
	} while (!this->empty());
}

/* Pops the element at the front of the `_elements` array */
void QCFSStream::next(QCFSBlock* block) {
	// if the `_elements` array points somewhere...
	if (_elements != nullptr) {
		QCFSBlock output = _elements[0];  // ...save the first element before it gets sliced away
		_shiftl_elements();  // move 1,2,3,4,... into 0,1,2,3...
		_resize_elements(--length);  // decrease the length and resize so that 0,1,2,n,n becomes 0,1,2,n
		memcpy_s((void*)block, sizeof(QCFSBlock), (void*)&output, sizeof(QCFSBlock));  // copy the first element that was saved then sliced away to the pointer `block`
	}
	// if not...
	else {
		memset((void*)block, 0, sizeof(QCFSBlock));  // ...zero out the block
	}
}


/* Resizes `_elements` array to size `newLength` */
void QCFSStream::_resize_elements(uint64_t newLength) {
	// If we are creating a non-empty array...
	if (newLength > 0) {
		// ...and we have an existing memory address
		if (_elements != nullptr) {
			_elements = (QCFSBlock*)realloc(_elements, sizeof(QCFSBlock) * newLength);  // Resize our existing pointer
		}
		// ...and we need to allocate a new memory address
		else {
			_elements = pallocs(QCFSBlock, newLength);  // Allocate a pointer to an array with `length` amount of `QCFSBlock`s
		}
	}
	else {
		_elements = nullptr;  // Empty the array
	}
}

/* Shifts the elements of the `elements` array one position to the left, discarding the first element */
void QCFSStream::_shiftl_elements() {
	// Only shift elements if there's room to shrink
	if (length > 1) {
		memcpy_s((void*)_elements,  // Destination: 0, 1, 2, 3, 4, 5, ... n - 1
			sizeof(QCFSBlock) * (length - 1),  // Destination Size: n - 1
			(void*)(_elements + 1),  // Source: 1, 2, 3, 4, 5, ... n
			sizeof(QCFSBlock) * (length - 1)  // Source Size: n - 1
		);
	}
	// If there's one or less elements, empty the array
	else {
		_elements = nullptr;
	}
}