# DOODLE

A Doodle Jump clone made in C++ with SFML 2.6.2.

## Play without building

Download the latest `DOODLE.zip` from the [Releases](https://github.com/DarkestVoyager/DOODLE/releases) page, extract it and run `DOODLE.exe`.

> Windows SmartScreen may show a warning because the executable isn't code-signed. Click **More info → Run anyway**.

## Controls

| Key | Action |
|---|---|
| `A` / `D` | Move left / right |
| `Space` | Start / play again |
| `F1` | Toggle debug hitboxes |

The character jumps automatically when landing on a platform. Climb as high as you can and don't fall!

## Features

- Infinite procedurally generated platforms
- Camera that follows the player upward
- Score and persistent high score (saved in `HIGH_SCORE.txt`)
- Custom hitboxes for the player and platforms
- Sound effects
- Game states: start screen, playing, game over with restart

## Building from source

### Requirements

- Windows 10/11 (64-bit)
- Visual Studio 2022 or newer with the **Desktop development with C++** workload

### Steps

1. Clone the repository:
   ```
   git clone https://github.com/DarkestVoyager/DOODLE.git
   ```
2. Open `DOODLE.slnx` in Visual Studio.
3. Select the **x64** platform and the **Debug** or **Release** configuration.
4. Build with `Ctrl+Shift+B`, then run with `F5`.

SFML is included in `External/SFML` (headers, `.lib` files and DLLs), so there is nothing else to install. A post-build step copies the SFML DLLs next to the `.exe` automatically.

### Project structure

```
DOODLE/            Source code and assets
External/SFML/     SFML 2.6.2 (include, lib, bin)
DOODLE.slnx        Visual Studio solution
```

### Included DLLs

`External/SFML/bin` contains the SFML 2.6.2 (64-bit) DLLs, in Debug (`-d`) and Release variants:

- `sfml-graphics`, `sfml-window`, `sfml-system`, `sfml-audio`
- `sfml-network` (part of the SFML package, not used by the game)
- `openal32.dll` (required by `sfml-audio`)

### Manual setup (only if the project settings are lost)

| Setting | Value |
|---|---|
| C/C++ → Additional Include Directories | `$(SolutionDir)External\SFML\include` |
| Linker → Additional Library Directories | `$(SolutionDir)External\SFML\lib` |
| Linker → Input (Debug) | `sfml-graphics-d.lib; sfml-window-d.lib; sfml-system-d.lib; sfml-audio-d.lib` |
| Linker → Input (Release) | `sfml-graphics.lib; sfml-window.lib; sfml-system.lib; sfml-audio.lib` |
| Build Events → Post-Build Event | `xcopy /y /d "$(SolutionDir)External\SFML\bin\*.dll" "$(OutDir)"` |

### Assets

The `assets/` folder must be in the working directory of the game. When running from Visual Studio, make sure **Debugging → Working Directory** points to the folder that contains `assets/`.

## Troubleshooting

- **White rectangles / textures don't load:** the working directory is wrong, see *Assets* above.
- **"sfml-graphics-d-2.dll was not found":** the post-build copy didn't run. Copy the DLLs from `External/SFML/bin` next to the `.exe` manually.
- **"openal32.dll was not found":** copy `openal32.dll` from `External/SFML/bin` next to the `.exe`.
- **Linker errors:** make sure the platform is **x64** and the library paths match the table above.
- **No sound:** check that the sound files are in `assets/` and that `openal32.dll` is next to the `.exe`.

## Tech

- C++17
- SFML 2.6.2 (graphics, window, system, audio)
- Visual Studio