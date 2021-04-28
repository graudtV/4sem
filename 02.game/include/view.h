#pragma once

#include "game_decls.h"
#include "console.h"
#include <vector>
#include <chrono>

namespace game {

struct KeyboardEvent {
	console::KeyboardSymbol key;
};

struct ViewResizeEvent {
	ExtentRange range;
};

class View {
public:
	using TimerCallback = std::function<void ()>;
	using KeyboardCallback = std::function<void (KeyboardEvent)>;
	using ViewResizeCallback = std::function<void (ViewResizeEvent)>;
	
	View() {}
	virtual ~View() {}

	virtual void draw_greeting_screen() = 0;
	virtual void draw_game_screen() = 0;
	virtual void draw_final_screen() = 0;

	virtual void draw_snake_segment(Coord2D coord) = 0;
	virtual void draw_rabbit_segment(Coord2D coord) = 0;
	virtual void clear_segment(Coord2D coord) = 0;

	/* available only on game screen */
	virtual void draw_score(int score) = 0;
	virtual void draw_record(int record) = 0;

	void subscribe_timer(TimerCallback cb, int timeout_ms);
	void subscribe_keyboard(KeyboardCallback cb);
	void subscribe_view_resize(ViewResizeCallback cb);

	virtual ExtentRange supported_map_range() = 0;
	void adjust_to_map_size(Extent2D map_size);

	virtual void run_event_loop() = 0;
	void interrupt_event_loop() { m_interrupt_loop = true; }

protected:
	using Clock = std::chrono::high_resolution_clock;
	using Timeout = Clock::duration;

	virtual void adjust_to_map_size_impl(Extent2D map_size) = 0;

	struct TimerEventEntry {
		TimerCallback callback;
		Timeout timeout;
		std::chrono::time_point<Clock> previous_callback_time;
	};

	std::vector<TimerEventEntry> m_timer_listeners;
	std::vector<KeyboardCallback> m_keyboard_listeners;
	std::vector<ViewResizeCallback> m_view_resize_listeners;
	bool m_interrupt_loop = false;
};

} // game namespace end