//
// Created by Eyeve on 10.03.2025.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"


namespace Leslie
{
	class Engine
	{
	  public:
		Engine();

		float evaluate();

	  private:
		Board board;
		float searchTime = 2.0f;
	};
}


#endif //ENGINE_H
