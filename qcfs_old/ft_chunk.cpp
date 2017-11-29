#include "ft_chunk.hpp"
#include <stdlib.h>
#include <string.h>

void ft_chunk(chunk* c)
{
	ft_data* data = palloc(ft_data);

	ft_entry* empty = palloc(ft_entry);
	memset((void*)(&empty->ent_data), 0x00, sizeof(empty->ent_data));
	empty->ft_id = (byte)ft_entry_id::BLANK;

	ft_entry* root = palloc(ft_entry);
	memset((void*)(&root->ent_data), 0x00, sizeof(root->ent_data));
	root->ft_id = (byte)ft_entry_id::ROOT_FOLDER;

	memcpy_s((void*)(&data->entries[0]), sizeof(ft_entry), (void*)root, sizeof(ft_entry));
	for (int i = 1; i < FT_ENTRYAMT; i++) {
		memcpy_s((void*)(&data->entries[i]), sizeof(ft_entry), (void*)empty, sizeof(ft_entry));
	}
	data->entry_count = 1;

	c->id = (byte)chunk_id::BASE_FT;
	c->exp_ptr = 0LL;
	memcpy_s((void*)c->data, DATA_MINUS_HEAD, (void*)data, DATA_MINUS_HEAD);
}