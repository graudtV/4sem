#ifndef CLI_VIEW_H_
#define CLI_VIEW_H_

class View {
public:
	View() {}
	virtual ~View() {}

	virtual void draw() = 0;
};

#endif // VIEW_H_