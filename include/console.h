#ifndef CONSOLE_H
#define CONSOLE_H

class Console {
private:
	std::string command = "";
	unsigned int cursor = 0;
	bool consoleOpen = false;
	std::shared_ptr<SDL_Event> e;
	SDL_Rect textrect;
	std::string text;
public:

	void open();
	bool isOpen();
};

#endif