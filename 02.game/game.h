#ifndef GAME_H_
#define GAME_H_

#include <memory>

namespace game {

class View;

struct MapExtent {
	int width;
	int height;
};

class GameStage {
public:
	GameStage(View *view) : m_view(view) {}
	virtual ~GameStage() {}

	virtual void init() {}
	virtual void tick() = 0;
	virtual void draw_frame() = 0;

	bool is_finished() const { return m_is_finished; }
	std::unique_ptr<GameStage> next() { return std::move(m_next); }
protected:
	View *m_view; // not owning access
	void finish_stage(std::unique_ptr<GameStage> next);
private:
	bool m_is_finished;
	std::unique_ptr<GameStage> m_next;
};

class Game final {
public:
	Game(View *view);
	void tick();
	void draw_frame();

	bool is_finished() const { return !m_stage; }
private:
	std::unique_ptr<GameStage> m_stage;
	View *m_view; // not owning access
};

} // game namespace end

#endif // GAME_H_