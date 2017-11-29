#include <iostream>
#include "QCFSRoot.h"

int main() {
	QCFSRoot root = QCFSRoot((uint32_t)QCFSBlockSizes::FOUR_KB, 64, 1, "My Test Volume");
	QCFSStream r = root.serialize();
	return 0;
}