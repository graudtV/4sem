#include "view.h"

namespace game {

void View::subscribe_timer(TimerCallback cb, int timeout_ms)
{
	auto time = std::chrono::high_resolution_clock::now();
	auto timeout = std::chrono::duration_cast<Timeout>(
		std::chrono::duration<int, std::micro>(timeout_ms)
		);
	m_timer_listeners.push_back({cb, timeout, time});
}

void View::subscribe_keyboard(KeyboardCallback cb)
	{ m_keyboard_listeners.push_back(cb); }

void View::subscribe_view_resize(ViewResizeCallback cb)
	{ m_view_resize_listeners.push_back(cb); }

void View::adjust_to_map_size(Extent2D map_size)
{
	auto supported_range = supported_map_range();
	if (map_size.width < supported_range.min_extent.width
		|| map_size.height < supported_range.min_extent.height
		|| map_size.width > supported_range.max_extent.width
		|| map_size.height > supported_range.max_extent.height)
		throw std::invalid_argument("View::adjust_to_map_size()");
	adjust_to_map_size_impl(map_size);
}

} // game namespace end