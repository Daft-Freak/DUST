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
    display/layer0.cpp
    display/layer2.cpp
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
        display_priority_default
        display_stat_flags
        display_layer0_char_base
        display_layer0_4bpp
        display_layer0_8bpp
        display_layer0_size0
        display_layer0_size1
        display_layer0_size2
        display_layer0_size3
        display_layer0_flip
        display_layer0_mode1
        display_layer0_mode2
        display_layer0_mode3
        display_layer0_mode4
        display_layer0_mode5
        display_layer0_char_base_invalid
        display_layer0_screen_base_invalid
        display_layer2_mode0
        display_layer2_mode1
        display_layer2_mode1_char_base
        display_layer2_mode1_wrap
        display_layer2_mode1_size1
        display_layer2_mode1_size2
        display_layer2_mode1_size3
        display_layer2_mode1_rotscale
        display_layer2_mode2
        display_layer2_mode3
        display_layer2_mode3_rotscale
        display_layer2_mode4
        display_layer2_mode4_pageflip
        display_layer2_mode4_rotscale
        display_layer2_mode5
        display_layer2_mode5_pageflip
        display_layer2_mode5_rotscale
    )

    foreach(TEST ${TESTS})
        add_gba_executable(${CMAKE_PROJECT_NAME}_${TEST} ${SOURCES})

        target_compile_definitions(${CMAKE_PROJECT_NAME}_${TEST} PRIVATE SINGLE_TEST=${TEST})
    endforeach()
endif()