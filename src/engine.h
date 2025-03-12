#ifndef ENGINE_H
#define ENGINE_H

#include "position.h"
#include "options.h"


namespace Leslie
{
	class Engine
	{
	  public:
		Engine();

		// call to start searching moves
		void go();
		// call to stop searching moves
		void stop();
		// set a new game position
		void set_position(std::string fen);

	  private:
		Position position;
		Options options;
	};
}

#endif
