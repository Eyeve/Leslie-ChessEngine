#include "lichess_shell.h"

#include "return_codes.h"

namespace Leslie {

LichessShell::LichessShell(int argc, char** argv) : engine_(Engine::Options{}) {
  (void)argc;
  (void)argv;
}

int LichessShell::Start() { return SUCCESS; }

const Engine::Options& LichessShell::GetOptions() const { return engine_.GetOptions(); }

Engine::Options& LichessShell::GetOptions() { return engine_.GetOptions(); }

}  // namespace Leslie
