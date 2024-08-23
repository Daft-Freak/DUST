cmake_minimum_required(VERSION 3.18)
project(DUST)

option(BUILD_SINGLE_TEST_ROMS "Build ROMs for each test" OFF)
option(MULTIBOOT "Build multiboot ROMs" OFF)

#====================
# ROM information
#====================

set(ROM_TITLE "DAFTS TESTS")
set(ROM_GAME_CODE "CDTE")
set(ROM_MAKER_CODE "DF")
set(ROM_VERSION 100)

set(SOURCES
    display/blend.cpp
    display/display.cpp
    display/layer0.cpp
    display/layer2.cpp
    display/mosaic.cpp
    display/objects.cpp
    display/window.cpp
    dma/dma.cpp
    common.cpp
    main.cpp
)

find_package(librom REQUIRED)
find_package(libmultiboot REQUIRED)
find_package(gba-hpp REQUIRED)
find_package(agbabi REQUIRED)

function(add_gba_executable TARGET_NAME TARGET_SOURCES)
    add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${ARGN})

    set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".elf")
    target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

    target_compile_options(${TARGET_NAME} PUBLIC -fdata-sections -ffunction-sections -fno-exceptions -mthumb)
    target_link_options(${TARGET_NAME} PUBLIC -Wl,-Map=${TARGET_NAME}.map,--gc-sections)

    if(MULTIBOOT)
        target_link_libraries(${TARGET_NAME} PRIVATE libmultiboot)
    else()
        target_link_libraries(${TARGET_NAME} PRIVATE librom)
    endif()

    target_link_libraries(${TARGET_NAME} PRIVATE agbabi gba-hpp)

    set_target_properties(${TARGET_NAME} PROPERTIES
        ROM_TITLE "${ROM_TITLE}"
        ROM_ID "${ROM_GAME_CODE}"
        ROM_MAKER "${ROM_MAKER_CODE}"
        ROM_VERSION ${ROM_VERSION}
    )

    install_rom(${TARGET_NAME})

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
        display_obj_regular
        display_obj_affine
        display_obj_affine_double
        display_obj_hidden
        display_obj_regular_8bpp
        display_obj_affine_8bpp
        display_obj_regular_hflip
        display_obj_regular_vflip
        display_obj_regular_hvflip
        display_obj_regular_8bpp_hflip
        display_obj_regular_8bpp_vflip
        display_obj_regular_8bpp_hvflip
        display_obj_affine_rotscale
        display_obj_affine_8bpp_rotscale
        display_obj_affine_double_rotscale
        display_obj_regular_4bpp_pal
        display_obj_regular_1d
        display_obj_regular_8bpp_1d
        display_obj_mixed
        display_obj_wrap_x
        display_obj_wrap_y
        display_obj_wrap_y_bug
        display_obj_priority
        display_obj_priority_bug
        display_obj_bmp_char_base
        display_obj_line_limit_regular_size0
        display_obj_line_limit_regular_size1
        display_obj_line_limit_regular_size2
        display_obj_line_limit_regular_size3
        display_obj_line_limit_affine_size0
        display_obj_line_limit_affine_size1
        display_obj_line_limit_affine_size2
        display_obj_line_limit_affine_size3
        display_obj_line_limit_affine_double_size0
        display_obj_line_limit_affine_double_size1
        display_obj_line_limit_affine_double_size2
        display_obj_line_limit_affine_double_size3
        display_obj_line_limit_regular_offscreen
        display_obj_line_limit_regular_hidden
        display_obj_line_limit_regular_size0_hblank_access
        display_obj_line_limit_regular_size2_hblank_access
        display_window_win0_bg
        display_window_win1_bg
        display_window_obj_win_bg
        display_window_obj_win_mixed_bg
        display_window_priority
        display_window_objects
        display_window_objects_mixed
        display_window_win0_bg_affine
        display_window_hblank_irq
        display_window_hblank_dma
        display_window_invalid_x1
        display_window_invalid_x2
        display_window_invalid_y1
        display_window_invalid_y2
        display_window_invalid_y2_no_disable
        display_mosaic_mode0_0
        display_mosaic_mode0_1_1
        display_mosaic_mode0_4_4
        display_mosaic_mode0_9_9
        display_mosaic_mode0_15_15
        display_mosaic_mode0_0_15
        display_mosaic_mode0_15_0
        display_mosaic_mode0_4_4_scroll
        display_mosaic_mode2_0
        display_mosaic_mode2_1_1
        display_mosaic_mode2_4_4
        display_mosaic_mode2_9_9
        display_mosaic_mode2_15_15
        display_mosaic_mode2_0_15
        display_mosaic_mode2_15_0
        display_mosaic_mode2_1_1_rotscale
        display_mosaic_mode2_4_4_rotscale
        display_mosaic_mode2_9_9_rotscale
        display_mosaic_mode2_15_15_rotscale
        display_mosaic_mode2_0_15_rotscale
        display_mosaic_mode2_15_0_rotscale
        display_mosaic_mode3_0
        display_mosaic_mode3_4_4
        display_mosaic_mode3_15_15
        display_mosaic_mode3_0_15
        display_mosaic_mode3_15_0
        display_mosaic_mode4_0
        display_mosaic_mode4_4_4
        display_mosaic_mode4_15_15
        display_mosaic_mode4_0_15
        display_mosaic_mode4_15_0
        display_mosaic_mode5_0
        display_mosaic_mode5_4_4
        display_mosaic_mode5_15_15
        display_mosaic_mode5_0_15
        display_mosaic_mode5_15_0
        display_mosaic_objects_regular_1_1
        display_mosaic_objects_regular_4_4
        display_mosaic_objects_regular_9_9
        display_mosaic_objects_regular_15_15
        display_mosaic_objects_regular_0_15
        display_mosaic_objects_regular_15_0
        display_mosaic_objects_affine_1_1
        display_mosaic_objects_affine_4_4
        display_mosaic_objects_affine_9_9
        display_mosaic_objects_affine_15_15
        display_mosaic_objects_affine_0_15
        display_mosaic_objects_affine_15_0
        display_mosaic_window
        display_blend_mode0
        display_blend_mode1
        display_blend_mode1_all
        display_blend_mode1_add
        display_blend_mode1_reversed
        display_blend_mode1_no_second
        display_blend_mode1_between
        display_blend_mode1_objects
        display_blend_mode2
        display_blend_mode2_backdrop
        display_blend_mode3
        display_blend_mode3_backdrop
        display_blend_window
        display_blend_object_trans
        display_blend_object_trans_enable
        display_blend_object_trans_priority

        dma_delay
        dma_priority
        dma_priority_reverse
        dma_priority_hblank
        dma_timer
        dma_timer_multi
    )

    foreach(TEST ${TESTS})
        # work out where this test lives
        string(REPLACE "_" ";" SPLIT_TEST ${TEST})
        list(GET SPLIT_TEST 0 TEST_GROUP)
        list(GET SPLIT_TEST 1 TEST_SUBGROUP)

        # special case(had to be one...)
        if(${TEST_GROUP} STREQUAL "display" AND ${TEST_SUBGROUP} STREQUAL "obj")
            set(TEST_SUBGROUP "objects")
        endif()

        if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/${TEST_GROUP}/${TEST_SUBGROUP}.cpp)
            set(TEST_SUBGROUP ${TEST_GROUP}) # fallback to default subgroup
        endif()

        # build test sources
        set(TEST_SOURCES
            ${TEST_GROUP}/${TEST_SUBGROUP}.cpp
            common.cpp
            main.cpp
        )

        add_gba_executable(${CMAKE_PROJECT_NAME}_${TEST} ${TEST_SOURCES})

        target_compile_definitions(${CMAKE_PROJECT_NAME}_${TEST} PRIVATE SINGLE_TEST=${TEST})
    endforeach()
endif()
