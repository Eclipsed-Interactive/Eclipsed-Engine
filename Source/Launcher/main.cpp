#include <windows.h>

#include "Core/Logger/DebugLogger.h"
#include "Core/PathManager.h"

#include <filesystem>
#include <string>
#include <shobjidl.h>

#include <fstream>


#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
   
}