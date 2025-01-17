cmake_minimum_required(VERSION 3.29)
project(DrawSomething)

set(CMAKE_CXX_STANDARD 20)

include_directories(deps/SDL/include deps/SDL_image/include deps/SDL_mixer/include deps/SDL_ttf/include)

link_directories(
        ${CMAKE_SOURCE_DIR}/deps/SDL/cmake-build-debug
        ${CMAKE_SOURCE_DIR}/deps/SDL_image/cmake-build-debug
        ${CMAKE_SOURCE_DIR}/deps/SDL_mixer/cmake-build-debug
        ${CMAKE_SOURCE_DIR}/deps/SDL_ttf/cmake-build-debug
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_executable(DrawSomething src/client/main.cpp
        deps/icon_resource.rc
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
        src/client/ui/menus/MainMenu.cpp
        src/client/ui/menus/MainMenu.h
        src/client/core/Rectangles.cpp
        src/client/core/Rectangles.h
        src/client/ui/components/Canvas.cpp
        src/client/ui/components/Canvas.h
        src/client/ui/menus/IngameMenu.cpp
        src/client/ui/menus/IngameMenu.h
        src/client/ui/menus/base/FullscreenMenu.cpp
        src/client/ui/menus/base/FullscreenMenu.h
        src/client/ui/menus/components/ingame/Panel.cpp
        src/client/ui/menus/components/ingame/Panel.h
        src/client/ui/menus/components/ingame/Header.cpp
        src/client/ui/menus/components/ingame/Header.h
        src/client/ui/menus/components/ingame/ColorSelector.cpp
        src/client/ui/menus/components/ingame/ColorSelector.h
        src/client/ui/menus/components/ingame/ToolSelector.cpp
        src/client/ui/menus/components/ingame/ToolSelector.h
        src/client/ui/components/element/OverlayButton.cpp
        src/client/ui/components/element/OverlayButton.h
        src/client/words/Words.cpp
        src/client/words/Words.h
        src/client/ui/menus/components/ingame/components/BrushSizeButton.cpp
        src/client/ui/menus/components/ingame/components/BrushSizeButton.h
        src/shared/core/Numbers.h
        src/shared/core/Numbers.cpp
        src/client/ui/menus/AuthMenu.cpp
        src/client/ui/menus/AuthMenu.h
        src/client/ui/cursors/Circular.cpp
        src/client/ui/cursors/Circular.h
        src/client/ui/cursors/Cursors.h
        src/client/ui/cursors/Cursors.cpp
)

add_executable(DrawSomethingServer src/server/main.cpp
        deps/icon_resource.rc
        src/shared/core/Strings.cpp
)

target_link_libraries(${PROJECT_NAME} SDL3 SDL3_image SDL3_mixer SDL3_ttf)
target_link_options(${PROJECT_NAME} PRIVATE "-static")

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
