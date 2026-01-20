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
#include "optimization.hpp"
#include "assembler.hpp"

int main(int argc, char** argv)
{
    // Check to make sure proper args are given
    if (argc != 3)
    {
        std::cerr << "Hex++ Compiler: Incorrect arguments. Correct arguments are:" << std::endl;
        std::cerr << "<input.hxpp> <output.hexpattern>" << std::endl;
        return EXIT_FAILURE;
    }

    // Print name of file being compiled
    compilation_message(std::string("Compiling \"") + argv[1] + "\"...");

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

    // Parse tokens, not in scope so allocator doesn't destruct
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
    std::vector<Pattern> patterns;
    bool found_non_integer_num;
    {
        Generator generator(prog);
        patterns = generator.generate();
        found_non_integer_num = generator.has_non_integer_num;
    }

    // Do post-gen optimization
    {
        Optimizer optimizer(patterns);
        patterns = optimizer.optimize();
    }

    // Assemble patterns into string output
    std::string code;
    {
        Assembler assembler(patterns);
        code = assembler.assemble();
    }

    // Write patterns to file
    {
        std::fstream output(argv[2], std::ios::out);
        output << code;
    }

    compilation_message("Compilation successful. Building with Hexagon...");

    // Prepare for hexagon building
    STARTUPINFOW si = {0};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {0};

    // Prepare command
    char args[256];
    strcpy(args, ".\\hexagon.exe build \"");
    strcat(args, argv[2]);
    strcat(args, "\"");

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

        // Print warning if code has non-integer
        if (found_non_integer_num)
        {
            compilation_warning(
                "Hexagon's provided /give command may not work properly, as the compiled patterns contained non-integer numerical reflections. If the spell from the /give command does not function fully, try replacing non-integer num literals with integers (e.g. 0.5 becomes 1/2) or convert the hexpattern output into patterns another way.", 0);
        }
    }
    else
    {
        DWORD errCode = GetLastError();

        // Check if just missing Hexagon
        if (errCode != 2)
        {
            wchar_t err[256];
            memset(err, 0, 256);
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);
            std::wcout << "Hex++ Compiler: Failed to build output with Hexagon. CreateProcessA error: "
                << err << "Hex++ Compiler: Error code: " << errCode << std::endl;
        }
        else
        {
            compilation_message("Couldn't find hexagon.exe. If hexagon.exe is placed in the same folder as the compiler's executable, it could output a /give command for a focus with the compiled spell here.");
        }
    }

    return EXIT_SUCCESS;
}