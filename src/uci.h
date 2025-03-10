//
// Created by Eyeve on 10.03.2025.
//

#ifndef UCI_H
#define UCI_H

#include <iostream>
#include <vector>

#include "move.h"


class UCI_Engine {
  public:
	UCI_Engine(int argc, char** argv);
	~UCI_Engine();

	void loop();

	void debug(bool isDebug);
	bool isready();
	void go(std::istringstream& is);
	void position(std::istringstream& is);
	void setoptions(std::istringstream& is);

	Move make_move(const Move& move);

  private:
	static constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	std::istream in;
	std::ostream out;
	bool debugMode;
};



#endif //UCI_H
