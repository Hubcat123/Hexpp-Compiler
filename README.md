This is a compiler for a high-level language called Hex++. It compiles down into hexpattern, a syntax used to represent patterns in the Minecraft mod Hex Casting.

The beginnings of this compiler were made by following the YouTuber Pixeled's series on making a compiler (theirs was into assembly instead of Hex Casting patterns). Channel: https://www.youtube.com/@pixeled-yt

This compiler is made to work with Hexagon. If the hexagon.exe is placed in the same directory as the exe for the compiler, the command line output will include the /give command that will give a focus with the compiled spell. Hexagon installation page: https://master-bw3.github.io/Hexagon/

# How to Use

Installation Video: https://www.youtube.com/watch?v=W4kXdzz89-w

Instructions:
1. Download: Download Hex++Compiler.exe
2. Open cmd: Open a Command Prompt and navigate to the directory containing the exe, or right-click in the folder containing the exe and click "Open in Terminal"
3. Run Compiler: In the terminal, type "./Hex++Compiler.exe \<input> \<output>", where \<input> is the file path of the file containing your Hex++ code, and \<output> is the file path of the file you want to output to (It will create a new file if one does not exist). The file path of both is just the name of the file, with the extension, if the files are in the same folder as the exe.
4. Get Output: The terminal will print out the /give command needed to get a focus with your spell if it can find hexagon as described below, which may be copied by selecting, then using RMB (instead of CTRL + C). The output file you specified will contain the hexpattern code of your program.

# Hex++ How-To

Details on how to make a program are in the docs folder on Hex++'s github. This includes grammer, syntax, functions, examples, etc. It is recommended you start with overview.md and continue from there if you're not already familiar with C-type languages. If you are, then examples.md may have enough information for you to learn the language quickly, and functions.md has a comprehensive list of the inbuilt functions provided.

# Hexagon

This compiler is made to work with Hexagon. Here's steps on how to use hexagon with this compiler:
1. Download exe: Download hexagon.exe from https://master-bw3.github.io/Hexagon/
2. Download Config: Download hexagon_config.toml from this project's github page
3. Move: Move hexagon.exe and hexagon_config.toml into the same folder as Hex++Compiler.exe

The /give command Hexagon generates will give the player running it a focus containing the list of patterns/iotas the compiler generated. If you have cheats enabled, you can type this command into the chat line and use it to get the focus. However, usually (in my experience) the command is too long for chat. In this case, you'll need to put it in a command block and power the command block to get the focus. Sometimes, the command may be even too long for the command block, in which case your only options are to manually compile the patterns in parts with Hexagon using the command line, or make a datapack to specifically give you the focus. This is pretty rare though, as command blocks can hold very long commands. Since this method requires using commands, you may not want to or be able to use it direcly in the world you intend to use the spell. In this case, you'll need to either input the spell manually, or if you have Hexal you can use the everbook to transfer the spell from a creative world into your survival world.

Note that when running with hexagon, the patterns may be compile differently. For example, instead of "Thanatos' Reflection" you will see "\<WEST qqaed>." This is because some patterns are not supported by Hexagon by name, or use a different name than in-game. This is the compiler using custom patterns so that Hexagon can correctly generate the /give command. Removing hexagon.exe from the folder or temporarily renaming it will cause the compiler to generate like normal. Later there will likely be a flag you can pass into the compiler to manually disable Hexagon genreation, too.

## Great Spells w/ Hexagon

That's all you need to use hexagon normally, however if your compiled spell requires greater spells, then you'll need to follow additional steps. This is because greater spells change their pattern stroke orders from world-to-world, so for the compiled spell to work in a particular world, Hexagon needs to know what those patterns look like. Setting these can be somewhat compilcated, here's a step-by-step on how to do it:
1. Open: Open the config file you downloaded above using some kind of text editor, such as notepad. You should see a list of entries which should look something like "'craft/battery' = 'aqqqaqwwaqqqqqeqaqqqawwqwqwqwqwqw'". The entries in there are Hexagon's default entries for the greater spells.
2. Find Pattern: Find the entry in the list relating to the greater spell you want to use. The name of the entry is on the left side of the '=', and may be fairly different from what the book calls the greater spell (In the example above, "craft/battery" is the entry for "Craft Phial")
3. Find New Pattern: Open the Minecraft world the spell you're compiling is meant to be used in. It is important it is specifically the world the spell will be used in, the spell will (most likely) only work in the world you use for this and following steps. Once in the world, find the pattern for the greater spell you want to use. If you don't have it already, you'll need to either find or cheat in an "Ancient Scroll" containing the spell you're looking for.
4. Transcribe Pattern: This is the most complex step, but there are a few ways to do it. You'll need to transcribe the pattern into a format Hexagon can understand. This is what the letters to the right of the '=' are. Here are two methods:
   1. Hex Studio (Simple): Go to https://master-bw3.github.io/Hex-Studio/ and draw your greater spell pattern with either an extra line at the end, or at the beginning. You'll see something appear on the left along the lines of "Pattern 'aeeewwweeeadqddqawadw'". Copy the collection of letters in the quotes, you can copy paste them through the export option or you can simply type them somewhere else. You'll then need to remove the first letter if you added an extra segment at the beginning, or the last one if you added a segment at the end. Make sure you're only adding one segment.
   2. Manual Conversion (Complex): The patterns are defined by the kinds of "turns" they make as you draw the pattern. There are 5 kinds of turns: a sharp left 'a', a shallow left 'q', going straight 'w', a shallow right 'e', and a sharp right 'd'.These letters are chosen because on a typical keyboard, they are layed out so they match with how they turn, this may help you remember which ones to use. Now, you must type out each letter as just described for each turn, starting from the first one you hit while drawing the pattern.
5. Fill Entry: Now that you have the string of letters, replace the letters to the right of the '=' for your greater spell entry with them, making sure to keep the quotes that were around them. Once this is done, save the file and everything should work.
