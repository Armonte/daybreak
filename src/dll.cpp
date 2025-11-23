/**
 * dll.cpp
 * By: tougafgc
 * Date: 22 November 2025
 *
 * DLL entrypoint that is injected into
 * MBAA.exe. Main purpose is to serve
 * as a method to load newLISP inside
 * of Melty, which newLISP can use
 * the proper libraries and tools to
 * manipulate the game. Functions in C/++
 * can also be exported to newLISP or ran
 * in parallel if need be.
 */
#include <windows.h>

// char *newlispEvalStr(char *code);
typedef char* (__stdcall *newlisp_eval_t)(const char *code);

// Global variables that are initialized in one function and
// freed in another.
HMODULE newlisp_handle = NULL;
newlisp_eval_t newlisp_eval = NULL;

// Simple message box to display information and then kill
// the program.
void melt_error(const char* message) {
  MessageBox(NULL, message, "Error!", MB_OK);
  exit(0);
}

// Loads newLISP.dll, containing "newLispEvalStr",
// which will serve as the entrypoint into leaving the
// C/++ realm, and into the Land of Lisp.
BOOL load_newlisp(LPCWSTR path) {
  newlisp_handle = LoadLibraryW(path);
  if (!newlisp_handle) melt_error("Load: could not load newlisp.dll!");

  newlisp_eval = (newlisp_eval_t)GetProcAddress(newlisp_handle, "newlispEvalStr");
  if (!newlisp_eval) melt_error("Cannot initialize newlispEvalStr!");

  return TRUE;
}

// Effectively the DLL's main method. Initializes newLISP and
// hands processing (within this thread) to newLISP. See
// the crescent/ directory for what happens next.
DWORD WINAPI server_wrapper(LPVOID hmodule) {
  if (!load_newlisp(L"daybreak\\newlisp.dll")) melt_error("Main: could not load newlisp!");

  // Starts server, this thread will continue to operate
  // on a while (connection-is-stable-and-open) loop
  newlisp_eval("(load \"daybreak/crescent.lsp\")");

  FreeLibrary(newlisp_handle);
  return 0;
}

// NOTE
// DO _NOT_ run any remotely intensive processes within this method,
// besides creating threads and moving program flow elsewhere. DllMain
// needs to intentionally be small or else it will not load/inject properly.
BOOL WINAPI DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved) {
  switch (reason) {
    case DLL_PROCESS_ATTACH:
      CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)server_wrapper, hmodule, 0, 0);
      break;
    case DLL_PROCESS_DETACH:
      // The newLISP server instance will exist even past closing MBAA.exe.
      // So, instead of messing with threading or other WinAPI related tasks,
      // newLISP's eval is not stateless- we can call a method to stop
      // the server since we've previously called (load crescent.lsp).
      newlisp_eval("(stop-server)");
      break;
    default:
      break;
  }

  return TRUE;
}
