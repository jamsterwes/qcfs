#include "root_chunk.h"
#include <string.h>
#include <stdlib.h>

void root_chunk(root_data* rinfo, chunk* c)
{
	c->id = (byte)chunk_id::BASE_ROOT;
	c->exp_ptr = 0LL;
	memset((void*)rinfo->_padding, 0x00, ROOT_PADDING);
	memcpy_s((void*)c->data, DATA_MINUS_HEAD, (void*)rinfo, DATA_MINUS_HEAD);
}