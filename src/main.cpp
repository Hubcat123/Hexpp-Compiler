#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <string>
#include <vector>
#include <map>

#include "util.hpp"
#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"
#include "optimization.hpp"
#include "assembler.hpp"
#include "hexagonCompat.hpp"


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

    // Check if hexagon exists in the same folder
    bool hexagon_exists=hexagonExist();

    if (hexagon_exists)
    {
        compilation_message("Found hexagon.exe. If compilation is successful, the output will be built with Hexagon automatically. Some compiled patterns may have Hexagon-specific alternatives.");
    }
    else
    {
        compilation_message("Couldn't find hexagon.exe. If hexagon.exe is placed in the same folder as the compiler's executable, it could output a /give command for a focus with the compiled spell here.");
    }

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
    {
        Generator generator(prog, hexagon_exists);
        patterns = generator.generate();
    }

    // Do post-gen optimization
    {
        Optimizer optimizer(patterns);
        patterns = optimizer.optimize();
    }

    // Assemble patterns into string output
    std::string code;
    {
        Assembler assembler(patterns, hexagon_exists);
        code = assembler.assemble();
    }

    // Write patterns to file
    {
        std::fstream output(argv[2], std::ios::out);
        output << code;
    }

    if (hexagon_exists)
    {
        compilation_message("Compilation successful. Building with Hexagon...");
        hexagonBuild(argv);
    }
    else
    {
        compilation_message("Compilation successful.");
    }

    return EXIT_SUCCESS;
}