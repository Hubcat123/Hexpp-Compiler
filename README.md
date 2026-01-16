This is a compiler for a high-level language called Hex++. It compiles down into hexpattern, a syntax used to represent patterns in the Minecraft mod Hex Casting.

The beginnings of this compiler were made by following the YouTuber Pixeled's series on making a compiler (theirs was into assembly instead of Hex Casting patterns). Channel: https://www.youtube.com/@pixeled-yt

This compiler is made to work with Hexagon. If the hexagon.exe is placed in the same directory as the exe for the compiler, the command line output will include the /give command that will give a focus with the compiled spell. Hexagon installation page: https://master-bw3.github.io/Hexagon/

How to Use

1. Download: Download Hex++Compiler.exe
2. Open cmd: Open a Command Prompt and navigate to the directory containing the exe, or right-click in the folder containing the exe and click "Open in Terminal"
3. Run Compiler: In the terminal, type "./Hex++Compiler.exe <input> <output>", where <input> is the file path of the file containing your Hex++ code, and <output> is the file path of the file you want to output to (It will create a new file if one does not exist). The file path of both is just the name of the file, with the extension, if the files are in the same folder as the exe.
4. Get Output: The terminal will print out the /give command needed to get a focus with your spell if it can find hexagon as described above, which may be copied by selecting, then using RMB (instead of CTRL + C). The output file you specified will contain the hexpattern code of your program.
