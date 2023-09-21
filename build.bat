REM general build script
@ECHO off
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files
SET filenames=
FOR /R %%f in (*.c) do (
    SET filenames=!filenames! %%f
)

SET program=acorn
SET compilerFlags=-g -Wvarargs -Wall -Werror

SET includeFlags=-Isource -Isource/modules/log -Isource/acornlib/ -Isource/platform/ -Isource/acornkernel -I%VULKAN_SDK%/include
SET linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib
SET defines=-D_DEBUG -D_CRT_SECURE_NO_WARNINGS

ECHO ">>>>>>>>>> Building %program% <<<<<<<<<<"
clang %filenames% %compilerFlags% -o bin/%program%.exe %defines% %includeFlags% %linkerFlags%