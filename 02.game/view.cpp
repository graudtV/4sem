#include "view.h"

namespace game {

void View::adjust_to_map_size(MapExtent map_size)
{
	auto min_size = min_map_size();
	auto max_size = max_map_size();

	if (map_size.width < min_size.width
		|| map_size.height < min_size.height
		|| map_size.width > max_size.width
		|| map_size.height > max_size.height)
		throw std::invalid_argument("View::adjust_to_map_size(): failed to resize view");
	adjust_to_map_size_impl(map_size);
}

} // game namespace end