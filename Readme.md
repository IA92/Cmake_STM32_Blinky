# Getting Started
## Setup
- Install CMake (Link: https://cmake.org/download/), i.e. Windows 64 installer cmake-3.29.0-rc3-windows-x86_64.msi
- Install MingW64 for Windows (Link: https://www.mingw-w64.org/downloads/)
    - Get the winlibs version (Link: https://winlibs.com/), e.g., GCC 11.4.0 + MinGW-w64 11.0.0 (UCRT) release 1
        - Extract winlibs-x86_64-posix-seh-gcc-11.4.0-mingw-w64ucrt-11.0.0-r1.zip and place "mingw64" in C:\
        - Add "C:\mingw64\bin" to path (environment variables)
- Download appropriate toolchain for your target device, e.g. arm (Link: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
    - For Windows, get the .exe file and install it, e.g. arm-gnu-toolchain-12.2.rel1-mingw-w64-i686-arm-none-eabi.exe
- Get the startup, linker and cmsis/device related include files for your device by generating a project from STM32CubeMX with Toolchain/IDE set to "Makefile" (Link: https://www.st.com/en/development-tools/stm32cubemx.html) Note:  We might be able to generate this file by ourselves, but not in this project
    - Copy startup, linker and cmsis/device related include files to the include folder, e.g.,
        - startup_stm32f401xe.s
        - STM32F401RETx_FLASH.ld
        - stm32f4xx.h
        - stm32f401xe.h
        - system_stm32f4xx.c
        - system_stm32f4xx.h
        - core_cm4.h
        - cmsis_version.h
        - cmsis_compiler.h
        - cmsis_gcc.h
        - mpu_armv7.h

## STM32 Documentation
- Nucleo-64 Boards User Manual: https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf
- STM32f401RE Datasheet: https://www.st.com/resource/en/datasheet/stm32f401re.pdf
- STM32F401xB/C and STM32F401xD/E Reference Manual: https://www.st.com/resource/en/reference_manual/rm0368-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
- Nucleo-XXXXRX Board Schematic: file:///C:/Users/Indri92/Downloads/en.MB1136-DEFAULT-C05_Schematic.pdf
- STM32CubeProgrammer Software Description: https://www.st.com/resource/en/user_manual/um2237-stm32cubeprogrammer-software-description-stmicroelectronics.pdf 
## Building the project
- Configure CMake by calling: cmake -G "MinGW Makefiles" -B build
- Build by calling: cmake --build build
## Flashing the .bin file to the memory of the device
- <path-to-STM32_Programmer_CLI.exe> -c port=SWD freq=4000 reset=HWrst -w <path-to-bin-file> <start-adddress-of-memory>
- Example: `C:\ST\STM32CubeIDE_1.14.1\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.1.100.202311100844\tools\bin\STM32_Programmer_CLI.exe -c port=SWD freq=4000  reset=HWrst -w D:\Data\Personal\Documents\Others\Cmake_STM32_Blinky\build\blinky.bin 0x8000000`
