#include "hexagonCompat.hpp"
#include <iostream>

bool hexagonExist(){
   
    #ifdef WINDOWS
     std::ifstream hexagon_check(".\\hexagon.exe");
     return hexagon_check.good();
    #elif defined(LINUX)
     std::ifstream hexagon_check("hexagon");
     return hexagon_check.good();
    #endif
}

void hexagonBuild(char** argv){
    #ifdef WINDOWS
    STARTUPINFOW si = {0};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi = {0};

        // Prepare command
        char args[256];
        strcpy(args, ".\\hexagon.exe build \"");
        strcat(args, argv[2]);
        strcat(args, "\" hexagon_config.toml");

        // Process output with hexagon
        if (CreateProcessA(
            NULL,
            args,
            NULL, NULL, FALSE, 0, NULL, NULL,
            (LPSTARTUPINFOA)&si, (LPPROCESS_INFORMATION)&pi))
        {
            // Wait for hexagon to finish
            WaitForSingleObject(pi.hProcess, INFINITE);

            // Close process
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            // Add spacing after hexagon output
            std::cout << std::endl;
        }
        else
        {
            DWORD errCode = GetLastError();
            wchar_t err[256];
            memset(err, 0, 256);
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);
            std::wcout << "Hex++ Compiler: Failed to build output with Hexagon. CreateProcessA error: "
                << err << "Hex++ Compiler: Error code: " << errCode << std::endl;
        }
    #elif defined(LINUX)
    pid_t pid=fork();
    if(pid==0){
        execl(
            "./hexagon",
            "hexagon",
            "build",
            argv[2],
            "hexagon_config.toml",
            (char*)NULL
        );
        exit(1);
    }
    else if(pid>0){
        int status;
        waitpid(pid,&status,0);
        
    }
     std::cout << std::endl;
    #endif 
     
}