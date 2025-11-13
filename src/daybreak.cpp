#include "daybreak.h"

Daybreak::Daybreak() {

}

void Daybreak::debug_msg(std::string msg) {
  if (DLL_DEBUG) printf("[DEBUG] %s\n", msg.c_str());
}

void Daybreak::die(std::string msg) {
  printf("[ERROR] %s\n", msg.c_str());
  exit(1);
}

void Daybreak::spawn_process(const char *path, Daybreak::ProcessType type) {
  STARTUPINFOA si{};
  PROCESS_INFORMATION pi{};

  si.cb = sizeof(si);

  BOOL res = FALSE;
  if (type == Daybreak::ProcessType::NO_ARGS)
    res = CreateProcessA((LPSTR)path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
  else
    res = CreateProcessA(NULL, (LPSTR)path, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

  if (!res) {
    char err[50];
    sprintf(err, "Could not start %s\n", path);
    MessageBox(NULL, err, "Error!", MB_OK);
    exit(1);
  }

  WaitForSingleObject(pi.hProcess, INFINITE);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

int Daybreak::find_melty() {
  HANDLE proc_snapshot;
  PROCESSENTRY32  proc_melty;
  BOOL proc_found;
  int pid = MELTY_NOT_FOUND;

  proc_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (proc_snapshot == INVALID_HANDLE_VALUE)
    return MELTY_NOT_FOUND;

  proc_melty.dwSize = sizeof(PROCESSENTRY32);
  proc_found = Process32First(proc_snapshot, &proc_melty);
  while (proc_found) {
    if (strcmp(MELTY_PROGRAM_NAME, proc_melty.szExeFile) == 0) {
      pid = proc_melty.th32ProcessID;
      break;
    }

    proc_found = Process32Next(proc_snapshot, &proc_melty);
  }

  CloseHandle(proc_snapshot);
  return pid;
}

void Daybreak::inject(std::string dll_path) {
  FARPROC loadlib_fx = GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");
  HANDLE melty_handle;
  HANDLE melty_thread;
  LPVOID buffer;

  std::wstring w_dll_path = std::wstring(dll_path.begin(), dll_path.end());
  size_t w_dll_len = (w_dll_path.length() + 1) * sizeof(wchar_t);
  int pid = MELTY_NOT_FOUND;

  debug_msg("Finding Melty...");
  while (!pid) pid = find_melty();
  if (!pid) die("Could not find MBAA.exe");

  debug_msg("Melty PID: " + std::to_string(pid));
  melty_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
  buffer = VirtualAllocEx(melty_handle, NULL, w_dll_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  WriteProcessMemory(melty_handle, buffer, w_dll_path.c_str(), w_dll_len, NULL);

  DWORD tid;
  melty_thread = CreateRemoteThread(melty_handle, 0, 0, (LPTHREAD_START_ROUTINE)loadlib_fx, buffer, 0, &tid);

  printf("handle:%p\tthread: %p\tbuffer:%p\ttid: %ld\n", melty_handle, melty_thread, buffer, tid);
  CloseHandle(melty_handle);
}
