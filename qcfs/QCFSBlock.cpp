#include "QCFSBlock.h"

std::string block_id_to_string(block_id id)
{
	switch (id) {
	case block_id::BASE_BT:
		return "BASE_BT";
	case block_id::BASE_FT:
		return "BASE_FT";
	case block_id::BASE_ROOT:
		return "BASE_ROOT";
	case block_id::BASE_DATA:
		return "BASE_DATA";
	case block_id::EXPANSION_BT:
		return "EXPANSION_BT";
	case block_id::EXPANSION_FT:
		return "EXPANSION_FT";
	case block_id::EXPANSION_ROOT:
		return "EXPANSION_ROOT";
	case block_id::EXPANSION_DATA:
		return "EXPANSION_DATA";
	case block_id::FREE:
		return "FREE";
	}
}
