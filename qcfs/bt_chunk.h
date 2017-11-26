#pragma once
#include "chunk.h"
#include "bt_entry.h"

const uint32_t BT_DATA_SIZE = sizeof(byte);
const uint32_t BT_BLOCKAMT = DATA_MINUS_HEAD / sizeof(bt_entry);
const uint32_t BT_PADDING = DATA_MINUS_HEAD % sizeof(bt_entry);

#pragma pack(push, 1)
struct bt_data {
	bt_entry entries[BT_BLOCKAMT];
	byte _padding[BT_PADDING];
};
#pragma pack(pop)

void bt_chunk(chunk* c);