#!/bin/bash
# Build script for Acorn Engine
set echo on

mkdir -p bin

# Get a list of all the C files
filenames=$(find . -type f -name "*.c")

program="acorn"
compilerFlags="-g -fPIC"

includeFlags="-Isource -Isource/modules/log -Isource/acornlib/ -I$VULKAN_SDL/include"
linkerFlags="-lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon -L/usr/X11R6/lib -Wl,-rpath,."
defines="-D_DEBUG"

echo ">>>>>>>>>> Building $program <<<<<<<<<<"
clang $filenames $compilerFlags -o bin/$program $defines $includeFlags $linkerFlags