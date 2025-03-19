#include "lichess_shell.h"

int main(int argc, char* argv[]) {
  leslie::LichessShell shell(argc, argv);

  int return_code = shell.Start();
  return return_code;
}
