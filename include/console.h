#ifndef CONSOLE_H
#define CONSOLE_H

class Console {
private:
	std::string command = "";
	unsigned int cursor = 0;
	bool consoleOpen = false;
	std::shared_ptr<SDL_Event> e;
public:

	void open();
	bool isOpen();
};

#endif