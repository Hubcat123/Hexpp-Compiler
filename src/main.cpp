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

int main(int argc, char** argv)
{
    // Check to make sure proper args are given
    if (argc < 3)
    {
        std::cerr << "Hex++ Compiler: Incorrect arguments. Correct arguments are: \"<input.hexpp> <output> [flags]\"" << std::endl;
        return EXIT_FAILURE;
    }

    // Print name of file being compiled
    std::cout << "Compiling \"" << argv[1] << '\"' << std::endl;

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
    NodePrint root;
    {
        Parser parser(tokens);
        std::optional<NodePrint> opt_root = parser.parse();
        if (opt_root.has_value())
        {
            root = opt_root.value();
        }
        else
        {
            compilation_error("Failed to parse tokens");
        }
    }

    // Generate hexes
    std::string code;
    {
        Generator generator(root);
        code = generator.generate();
    }

    // Print out hex code
    std::cout << code << std::endl;

    // Write code to file
    {
        std::fstream output(argv[2], std::ios::out);
        output << code;
    }

    return EXIT_SUCCESS;
}