cmake_minimum_required(VERSION 3.29)
project(DrawSomething)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        C:/libs/SDL/include
        C:/libs/SDL_image/include
        C:/libs/SDL_mixer/include
        C:/libs/SDL_ttf/include
        C:/libs/asio/asio/include
)

link_directories(
        C:/libs/SDL/cmake-build-debug
        C:/libs/SDL_image/cmake-build-debug
        C:/libs/SDL_mixer/cmake-build-debug
        C:/libs/SDL_ttf/cmake-build-debug
#        ${CMAKE_SOURCE_DIR}/deps/SDL/cmake-build-debug
#        ${CMAKE_SOURCE_DIR}/deps/SDL_image/cmake-build-debug
#        ${CMAKE_SOURCE_DIR}/deps/SDL_mixer/cmake-build-debug
#        ${CMAKE_SOURCE_DIR}/deps/SDL_ttf/cmake-build-debug
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_executable(DrawSomething src/client/main.cpp
        res/client.rc
        src/shared/core/Strings.cpp
        src/client/core/app/Assets.cpp
        src/client/core/app/Assets.h
        src/client/core/app/Clock.cpp
        src/client/core/app/Clock.h
        src/client/core/app/Drawing.cpp
        src/client/core/app/Drawing.h
        src/client/core/Application.cpp
        src/client/core/Application.h
        src/client/ui/components/Scribbles.cpp
        src/client/ui/components/Scribbles.h
        src/client/ui/components/element/Button.h
        src/client/ui/components/element/TextBox.cpp
        src/client/ui/components/element/base/Element.cpp
        src/client/ui/components/element/base/Element.h
        src/client/ui/components/element/Button.cpp
        src/client/ui/components/element/TextBox.h
        src/client/ui/components/event/EventContext.h
        src/client/ui/components/event/EventContext.cpp
        src/client/ui/components/element/Frame.cpp
        src/client/ui/components/element/Frame.h
        src/client/ui/menus/main/MainMenu.cpp
        src/client/ui/menus/main/MainMenu.h
        src/client/core/Rectangles.cpp
        src/client/core/Rectangles.h
        src/client/ui/menus/ingame/components/Canvas.cpp
        src/client/ui/menus/ingame/components/Canvas.h
        src/client/ui/menus/ingame/IngameMenu.cpp
        src/client/ui/menus/ingame/IngameMenu.h
        src/client/ui/menus/FullscreenMenu.cpp
        src/client/ui/menus/FullscreenMenu.h
        src/client/ui/menus/ingame/components/panel/Panel.cpp
        src/client/ui/menus/ingame/components/panel/Panel.h
        src/client/ui/menus/ingame/components/Header.cpp
        src/client/ui/menus/ingame/components/Header.h
        src/client/ui/menus/ingame/components/color_selector/ColorSelector.cpp
        src/client/ui/menus/ingame/components/color_selector/ColorSelector.h
        src/client/ui/menus/ingame/components/tool_selector/ToolSelector.cpp
        src/client/ui/menus/ingame/components/tool_selector/ToolSelector.h
        src/client/ui/components/element/OverlayButton.cpp
        src/client/ui/components/element/OverlayButton.h
        src/client/words/Words.cpp
        src/client/words/Words.h
        src/client/ui/menus/ingame/components/tool_selector/BrushSizeButton.cpp
        src/client/ui/menus/ingame/components/tool_selector/BrushSizeButton.h
        src/shared/core/Numbers.h
        src/client/ui/menus/auth/AuthMenu.cpp
        src/client/ui/menus/auth/AuthMenu.h
        src/client/ui/cursors/Circular.cpp
        src/client/ui/cursors/Circular.h
        src/client/ui/cursors/Cursors.h
        src/client/ui/cursors/Cursors.cpp
        src/client/ui/structures/WrappedText.cpp
        src/client/ui/structures/WrappedText.h
        src/client/ui/structures/window_texture/WindowTexture.cpp
        src/client/ui/structures/window_texture/WindowTexture.h
        src/shared/core/Colors.cpp
        src/shared/core/Colors.h
        src/client/ui/structures/window_texture/Sheet.cpp
        src/client/ui/structures/window_texture/Sheet.h
        src/client/ui/menus/ingame/components/panel/GuessingBar.cpp
        src/client/ui/menus/ingame/components/panel/GuessingBar.h
        src/client/ui/menus/ingame/components/panel/Letters.cpp
        src/client/ui/menus/ingame/components/panel/Letters.h
        src/client/ui/menus/ingame/ingame_common.h
        src/client/ui/menus/ingame/components/panel/Letter.cpp
        src/client/ui/menus/ingame/components/panel/Letter.h
        src/client/ui/menus/ingame/components/panel/LetterSlot.cpp
        src/client/ui/menus/ingame/components/panel/LetterSlot.h
)

add_executable(DrawSomethingServer src/server/main.cpp
        res/server.rc
        src/shared/core/Strings.cpp
        src/shared/Protocol.h
        src/shared/network/ThreadQueue.h
        src/shared/network/Message.h
        src/shared/network/Connection.h
        src/server/network/IServer.h
)

add_executable(DrawSomethingTestClient src/client/test_client.cpp
        res/client.rc
        src/shared/core/Strings.cpp
        src/shared/Protocol.h
        src/shared/network/ThreadQueue.h
        src/shared/network/Message.h
        src/shared/network/Connection.h
        src/client/network/IClient.h
)

target_link_libraries(${PROJECT_NAME} SDL3 SDL3_image SDL3_mixer SDL3_ttf)
target_link_options(${PROJECT_NAME} PRIVATE "-static")

target_link_libraries(DrawSomethingServer ws2_32 wsock32)
target_link_options(DrawSomethingServer PRIVATE "-static")
target_link_libraries(DrawSomethingTestClient ws2_32)
target_link_options(DrawSomethingTestClient PRIVATE "-static")

file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

function(copy_files SOURCE_DIR DEST_DIR)
    file(GLOB items "${SOURCE_DIR}/*")
    foreach (item ${items})
        if (IS_DIRECTORY ${item})
            get_filename_component(dirname ${item} NAME)
            file(MAKE_DIRECTORY ${DEST_DIR}/${dirname})
            copy_files(${item} ${DEST_DIR}/${dirname})
        else ()
            get_filename_component(filename ${item} NAME)
            configure_file(${item} ${DEST_DIR}/${filename} COPYONLY)
        endif ()
    endforeach ()
endfunction()

copy_files(${CMAKE_CURRENT_SOURCE_DIR}/bin ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
