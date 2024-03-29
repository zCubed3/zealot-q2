#
# Executable variables
#
file(GLOB_RECURSE CLIENT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp
)

include(${CMAKE_SOURCE_DIR}/server/CMakeLists.txt)

list(APPEND CLIENT_SRCS
    ${SERVER_SRCS}

    "${QCOMMON_DIR}/crc.cpp"
    "${QCOMMON_DIR}/cmodel.cpp"
    "${QCOMMON_DIR}/common.cpp"
    "${QCOMMON_DIR}/files.cpp"
    "${QCOMMON_DIR}/net_chan.cpp"
    "${QCOMMON_DIR}/pmove.cpp"
    "${QCOMMON_DIR}/md4.cpp"

    "${CMAKE_SOURCE_DIR}/games/quake2/q_shared.cpp"
    "${CMAKE_SOURCE_DIR}/games/quake2/m_flash.cpp"

    "${CMAKE_SOURCE_DIR}/platforms/sdl/vid_sdl.cpp"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/vid_menu.cpp"

    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_input_sdl.cpp"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_system_sdl.cpp"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_shared_sdl.cpp"

    "${CMAKE_SOURCE_DIR}/platforms/null/cd_null.cpp" # TODO: Replace CD Audio!

    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_demo.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_sdl2.cpp"
)

list(APPEND CLIENT_LIBS
    ZealCore
    Zealot_Core
    SDL2
    SDL2main
)

list(APPEND CLIENT_INC_DIRS
    ZealCore
    SDL2
    ${CMAKE_SOURCE_DIR}/third_party/imgui
)

list(APPEND CLIENT_DEFS
    ${ZEALOT_GDEFS}
)

#
# Platform specifics
#
if (WIN32)
    list(APPEND CLIENT_SRCS
        "${CMAKE_SOURCE_DIR}/platforms/sdl/win32/net_wins.cpp"
        "${CMAKE_SOURCE_DIR}/platforms/sdl/win32/snd_win.cpp"

        "${CMAKE_SOURCE_DIR}/platforms/sdl/q2.rc"
    )

    list(APPEND CLIENT_LIBS
        "winmm"
        "ws2_32"
    )
endif()

if (APPLE)
    list(APPEND CLIENT_SRCS
        "${CMAKE_SOURCE_DIR}/platforms/sdl/posix/net_posix.cpp"
        "${CMAKE_SOURCE_DIR}/platforms/null/snddma_null.cpp"
    )
endif()

#
# Executable definition
#
add_executable(Zealot_Client ${CLIENT_SRCS})

target_link_libraries(Zealot_Client PRIVATE ${CLIENT_LIBS})
target_include_directories(Zealot_Client PRIVATE ${CLIENT_INC_DIRS})
target_compile_definitions(Zealot_Client PRIVATE ${CLIENT_DEFS})

set_target_properties(Zealot_Client PROPERTIES OUTPUT_NAME "${ZEALOT_CLIENT_NAME}")

#add_dependencies(Zealot_Client Zealot_RefGL2 Zealot_Game)