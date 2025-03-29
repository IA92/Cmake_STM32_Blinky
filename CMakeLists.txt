cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchain.cmake")

project(Cmake_STM32_Blinky)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(blinky 
                    src/main.c 
                    src/system_config.c
                    ${CMAKE_SOURCE_DIR}/include/device/startup_stm32f401xe.s
                    ${CMAKE_SOURCE_DIR}/include/device/system_stm32f4xx.c
)

target_compile_definitions(blinky PRIVATE -DSTM32F401xE)

target_include_directories(blinky PRIVATE
                    ${CMAKE_SOURCE_DIR}/include/
                    ${CMAKE_SOURCE_DIR}/include/device
)

# This is from STM32CubeMX generated makefile
target_compile_options(blinky PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -DSTM32F401xE

        -fdata-sections
        -ffunction-sections

        -Wall

        $<$<CONFIG:Debug>:-Og>
        )

# This is from STM32CubeMX generated makefile
set(STM32F401RE_LINK_FLAGS 
                    "-mcpu=cortex-m4 \
                    -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
                    -specs=nano.specs \
                    -T${CMAKE_SOURCE_DIR}/include/device/STM32F401RETx_FLASH.ld -lc -lm -lnosys \
                    -Wl,-Map=${CMAKE_BINARY_DIR}/blinky.map,--cref \
                    -Wl,--gc-sections"
)
set_target_properties(blinky PROPERTIES LINK_FLAGS ${STM32F401RE_LINK_FLAGS})

# This is from an online tutorial (Link: https://dev.to/pgradot/cmake-on-stm32-the-beginning-3766)
# Instruction to load the file to the flash can be found here https://www.st.com/resource/en/user_manual/um2576-stm32cubeide-stlink-gdb-server-stmicroelectronics.pdf
add_custom_command(TARGET blinky
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex blinky blinky.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary blinky blinky.bin)