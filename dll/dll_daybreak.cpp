#include <windows.h>

static PROCESS_INFORMATION pi{};
static HANDLE job = NULL;

void melt_error(const char *message) {
  MessageBox(NULL, message, "Error!", MB_OK);
  exit(1);
}

void spawn_lisp() {
  STARTUPINFOA si{};
  const char *path = "daybreak\\newlisp.exe -c -p 8080";

  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;

  if (!CreateProcessA(NULL, (LPSTR)path, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
    melt_error("Could not start newLISP!");
  }

  // newLISP is a separate process, so to prevent it living past the lifetime
  // of MBAA.exe, kill it when the DLL detaches from the EXE.
  job = CreateJobObjectA(NULL, NULL);
    if (job) {
      JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = {0};
      jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
      SetInformationJobObject(job, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));
      AssignProcessToJobObject(job, pi.hProcess);
    }

    ResumeThread(pi.hThread);
}

BOOL WINAPI DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved) {
  HANDLE lisp_handle = NULL;

  switch (reason) {
    case DLL_PROCESS_ATTACH:
      lisp_handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)spawn_lisp, 0, 0, 0);
      break;

    case DLL_PROCESS_DETACH:
      if (lisp_handle) CloseHandle(lisp_handle);
      if (pi.hThread)  CloseHandle(pi.hThread);
      if (job)         CloseHandle(job);

      break;
    default:
      break;
  }

  return TRUE;
}
