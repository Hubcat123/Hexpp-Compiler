# Compile Instructions

## Dependenсies

### Windows
1. Mingw64 or Visual Studio 17 2022.
2. Required tools in PATH:
- CMake(version >=3.21).
- gcc, g++, gdb, Ninja if you use Mingw64.
- cl if you use Visual Studio.

### Linux
1. CMake (version >=3.21).
2. gcc, g++, gdb.
3. Ninja.
   
### *Optionally
- hexagon for you platform in project root
https://github.com/Master-Bw3/Hexagon.
- test file on project root(test.hxpp).

### Visual Studio Code dependencies
1. C/C++ extension.
2. CMake tools extension.

## Visual Studio Code
1. Clone or download the repository.
2. Open the repo folder in Visual Studio Code.
3. Select a preset in Run and Debug.
4. Launch the vscode preset for your platform.

## CMake
1. Clone or download the repository.
2. Open the repo folder in console.
3.
```powershell
cmake --preset <Cmake Preset Name>
```
4.
```powershell
cmake --build --preset <Cmake Preset Name>
```


## Notes
- if errors occur, check for gcc, g++, gdb(Mingw64) or cl(Visual Studio) in PATH.
- Hexagon is not involved in the project build and is used only as a compilation utility.
