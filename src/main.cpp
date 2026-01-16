#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <string>
#include <vector>
#include <map>

#include <windows.h>

#include "util.hpp"
#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"

int main(int argc, char** argv)
{
    // Check to make sure proper args are given
    if (argc < 3)
    {
        std::cerr << "Hex++ Compiler: Incorrect usage. Correct usage is:" << std::endl;
        std::cerr << "Hex++Compiler <input.hexpp> <output> [flags]" << std::endl;
        return EXIT_FAILURE;
    }

    // Print name of file being compiled
    std::cout << "Compiling \"" << argv[1] << "\"..." << std::endl;

    // Read contents of file to compile into a string
    std::string contents;
    {
        std::fstream input(argv[1], std::ios::in);
        std::stringstream contents_stream;
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    // Tokenize the contents
    std::vector<Token> tokens;
    {
        Tokenizer tokenizer(std::move(contents));
        tokens = tokenizer.tokenize();
    }

    // Parse tokens
    NodeProg* prog;
    Parser parser(tokens);
    std::optional<NodeProg*> opt_prog = parser.parse();
    if (opt_prog.has_value())
    {
        prog = opt_prog.value();
    }
    else
    {
        compilation_error("Failed to parse tokens", 0);
    }

    // Generate hexes
    std::string code;
    {
        Generator generator(prog);
        code = generator.generate();
    }

    // Write patterns to file
    {
        std::fstream output(argv[2], std::ios::out);
        output << code;
    }

    // Prepare for hexagon building
    STARTUPINFOW si = {0};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {0};

    // Prepare command
    char args[256];
    strcpy(args, "..\\hexagon.exe build \"");
    strcat(args, argv[2]);
    strcat(args, "\"");

    std::cout << "Compilation successful. Building with Hexagon..." << std::endl;

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
    }
    else
    {
        DWORD errCode = GetLastError();
        wchar_t err[256];
        memset(err, 0, 256);
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);
        std::wcout << "Failed to build output with Hexagon. Is the exe missing or in the wrong place? CreateProcessA error: "
            << err << "Error code: " << errCode << std::endl;
    }

    return EXIT_SUCCESS;
}