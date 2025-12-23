# Swarm

A swarm simulation written in C.

## Features
- SDL2-based rendering and simulation
- Hash grid utilities
- Swarm behavior implementation
- Example C

## Build Instructions
1. Install [MSYS2](https://www.msys2.org/) and ensure `ucrt64` environment is set up.
2. Install SDL2 libraries in MSYS2.
3. Build using the provided VS Code task or run:
   ```sh
   gcc *.c -o main.exe -g -O -Wall -Wextra -std=c17 -LC:/msys64/ucrt64/lib -IC:/msys64/ucrt64/include/SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
   ```
## License
This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
