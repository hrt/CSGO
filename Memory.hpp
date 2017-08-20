#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <algorithm>
#define TARGET_PROCESS "csgo.exe"
#define CLIENT_DLL_NAME "client.dll"
#define ENGINE_DLL_NAME "engine.dll"

class Memory
{

private:
  uint32_t PID;
  HANDLE hProcess;
  uint32_t getProcessId(const std::string& processName);
  bool attachToProcess(const std::string& processName);
  uint32_t getModule(const char* moduleName);
public:
  uint32_t clientDll, engineDll;

  // RPM wrapper
  template<class T>
  T read(uint32_t address)
  {
    T buffer;
    ReadProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(T), 0);
    return buffer;
  }

  // WPM wrapper
  template<class T>
  BOOL write(uint32_t address, T buffer)
  {
    return WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(T), 0);
  }

  Memory();
  ~Memory();

};
