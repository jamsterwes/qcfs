#include "bt_chunk.hpp"
#include <string.h>
#include <stdlib.h>

void bt_chunk(chunk* c)
{
	bt_data* data = palloc(bt_data);
	memset((void*)data->entries, (int)chunk_id::FREE, BT_BLOCKAMT);

	c->id = (byte)chunk_id::BASE_BT;
	c->exp_ptr = 0LL;
	memcpy_s((void*)c->data, DATA_MINUS_HEAD, (void*)data, DATA_MINUS_HEAD);
}