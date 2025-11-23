# Melty Blood: Daybreak

Daybreak aims at taking the many individual utilites for Melty Blood: AACC and making them
available altogether inside the game's base executable. In addition, an interactive scripting
environment is provided by the newLISP language for easy extensibility without the need
for recompilation.

[Play Melty Blood](https://play.meltyblood.club/) | [newLISP](http://www.newlisp.org)

## Installation

*Please compile using MinGW32*, see "Hacking & Modification" for more information.
Currently, only the source is available. To compile,

1. Download this repository
2. run `download_deps.sh` to download MBAACC and newLISP
3. run `make all` to generate the executables

Now, within the "out" folder, there is `daybreak.exe` and a daybreak folder. These
can be copied to any installation of MBAACC, but for easy testing, they are automatically
placed in the `mbaa/` folder. 

## Usage

Either by double-clicking `daybreak.exe`, running it in the command line, or typing `make test` 
MBAACC will automatically open with all modules preloaded.

## Hacking & Modification

This project is dependent only on MinGW32. No UCRT or MSVC binaries or tools are used in the
development or compilation of Daybreak. Installing [MSYS2](https://www.msys2.org/)
and the x86 gcc/g++ toolchain are the only requirements to get it working properly. 

*The newLISP server provided in Daybreak listens to `localhost:25565`, and is currently hardcoded to listen here.*
*client.lsp and its compiled client.exe will automatically look for this address and connect to each other.*

## Roadmap

- Write text inside of Melty
- Contain the newLISP client inside of Melty (open/closing by pressing a button combination)
- Controller mappings
- button test menu
