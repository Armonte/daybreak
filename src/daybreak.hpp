/**
 * daybreak.hpp
 * By: tougafgc
 * Date: 22 November 2025
 *
 * Methods pertaining to recognizing
 * and injecting the DLL into Melty Blood.
 **/
#ifndef DAYBREAK_HPP_
#define DAYBREAK_HPP_

#include <cstdio>
#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

#define DLL_DEBUG 1

#define MELTY_NOT_FOUND    0
#define MELTY_FOUND        1
#define MELTY_PROGRAM_NAME "MBAA.exe"

class Daybreak {
  public:
    Daybreak();
    ~Daybreak() = default;

    // Debug console message, only appears if DLL_DEBUG = 1
    void debug_msg(std::string message);

    // Error console message, kills program flow
    void die(std::string msg);

    // Find an MBAA.exe process and inject Daybreak
    void hook(std::string dll_path);

  private:
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // Take a snapshot of the current open processes
    // and find the PID of Melty
    int  find_melty();

    // Open Melty
    void spawn_process(const char *path);

    void inject(std::string dll_path);

    // ASM injection that skips the configuration menu
    // for Melty
    void skip_config();
};

#endif // DAYBREAK_HPP_
