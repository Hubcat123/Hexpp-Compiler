#pragma once
#ifdef WINDOWS
#include <windows.h>
#elif defined(LINUX)
#include <unistd.h>
#include <sys/wait.h>
#endif
#include <fstream>

bool hexagonExist();
void hexagonBuild(char** argv);