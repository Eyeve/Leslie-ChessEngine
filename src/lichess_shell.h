#ifndef UCI_H
#define UCI_H

#include <iostream>

#include "move.h"
#include "engine.h"


namespace Leslie
{
	class LichessShell {
      public:
        LichessShell(int argc, char** argv);
        ~LichessShell();

        int start();

		const Options& get_options() const;
		Options& get_options();

      private:
	    Engine engine;
    };
}

#endif
