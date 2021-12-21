#pragma once
#ifdef __DLLEXPORT
#define __DLL_EXP _declspec(dllexport) 
#else
#define __DLL_EXP _declspec(dllimport) 
#endif 
#include "pch.h"
extern "C" __DLL_EXP void Init();
extern "C" __DLL_EXP void Open(const char*);
extern "C" __DLL_EXP void Write(LOGLEVEL type, const char* log);
extern "C" __DLL_EXP void Close();
extern "C" __DLL_EXP void Uninit();

