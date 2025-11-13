#include "daybreak.h"

void start_melty() {
  Daybreak daybreak;
  daybreak.spawn_process("MBAA.exe", Daybreak::ProcessType::NO_ARGS);
}

int main(int argc, char *argv[]) {
  std::filesystem::path pwd = std::filesystem::current_path();
  std::filesystem::path full_path = pwd / "daybreak\\melty_scheme.dll";

  Daybreak daybreak;

  if (std::filesystem::exists(full_path)) {
    daybreak.debug_msg(full_path.string().c_str());
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_melty, 0, 0, 0);
    daybreak.inject(full_path.string());
  }

  return 0;
}
