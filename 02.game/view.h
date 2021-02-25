#ifndef CLI_VIEW_H_
#define CLI_VIEW_H_

namespace game {

class View {
public:
	View() {}
	virtual ~View() {}

	virtual void draw() = 0;
};

} // game namespace end

#endif // VIEW_H_