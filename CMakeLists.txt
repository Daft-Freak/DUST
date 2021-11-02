cmake_minimum_required(VERSION 3.8)
project(DUST)

option(BUILD_SINGLE_TEST_ROMS "Build ROMs for each test" OFF)

#====================
# ROM information
#====================

set(ROM_TITLE "DAFTS TESTS")
set(ROM_GAME_CODE "CDTE")
set(ROM_MAKER_CODE "DF")
set(ROM_VERSION 100)

set(SOURCES
    display/display.cpp
    common.cpp
    main.cpp
)

function(add_gba_executable TARGET_NAME TARGET_SOURCES)
    add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${ARGN})

    set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".elf")
    target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

    target_compile_options(${TARGET_NAME} PUBLIC -fdata-sections -ffunction-sections -fno-exceptions)
    target_link_options(${TARGET_NAME} PUBLIC --specs=nano.specs -Wl,-Map=${TARGET_NAME}.map) # rom/runtime.specs looks like it should be using -lc_nano, but doing this removes ~1k

    gba_target_link_agb_abi(${TARGET_NAME})
    gba_target_link_gba_plusplus(${TARGET_NAME})

    gba_target_sources_instruction_set(${TARGET_NAME} thumb)
    gba_target_link_runtime(${TARGET_NAME} rom)

    gba_target_object_copy(${TARGET_NAME} "${TARGET_NAME}.elf" "${TARGET_NAME}.gba")
    gba_target_fix(${TARGET_NAME} "${TARGET_NAME}.gba" "${ROM_TITLE}" "${ROM_GAME_CODE}" "${ROM_MAKER_CODE}" ${ROM_VERSION})
endfunction()

# combined suite ROM
add_gba_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# single test ROMs
if(BUILD_SINGLE_TEST_ROMS)
    set(TESTS
        display_layer_none
        display_cgb_mode
        display_forced_blank
    )

    foreach(TEST ${TESTS})
        add_gba_executable(${CMAKE_PROJECT_NAME}_${TEST} ${SOURCES})

        target_compile_definitions(${CMAKE_PROJECT_NAME}_${TEST} PRIVATE SINGLE_TEST=${TEST})
    endforeach()
endif()