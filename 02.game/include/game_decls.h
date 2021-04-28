#ifndef GAME_DECLS_H_
#define GAME_DECLS_H_

namespace game {

struct Coord2D {
	int x = 0;
	int y = 0;
};

inline bool operator ==(Coord2D fst, Coord2D snd) { return fst.x == snd.x && fst.y == snd.y; }
inline bool operator !=(Coord2D fst, Coord2D snd) { return !(fst == snd); }

struct Extent2D {
	int width = 0;
	int height = 0;
};

struct ExtentRange {
	Extent2D min_extent;
	Extent2D max_extent;
};

struct Rect2D {
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	Rect2D() {}
	Rect2D(Coord2D top_left_corner, Extent2D extent) :
		x(top_left_corner.x),
		y(top_left_corner.y),
		width(extent.width),
		height(extent.height) {}

	int xend() const { return x + width; }
	int yend() const { return y + height; }
	Coord2D coord() const { return {x, y}; }
	Extent2D extent() const { return {width, height}; }
};

} // game namespace end

#endif // GAME_DECLS_H_