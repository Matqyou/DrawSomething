cmake_minimum_required(VERSION 3.29)
project(DrawSomething)

set(CMAKE_CXX_STANDARD 20)

include_directories(deps/SDL3/include deps/SDL2_image/include deps/SDL2_mixer/include deps/SDL2_ttf/include)
link_directories(deps/SDL3/lib/x64 deps/SDL2_image/lib/x64 deps/SDL2_mixer/lib/x64 deps/SDL2_ttf/lib/x64)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_executable(DrawSomething src/main.cpp
        deps/icon_resource.rc
        src/client/core/app/Assets.cpp
        src/client/core/app/Assets.h
        src/client/core/app/Clock.cpp
        src/client/core/app/Clock.h
        src/client/core/app/Drawing.cpp
        src/client/core/app/Drawing.h
        src/client/core/Application.cpp
        src/client/core/Application.h
        src/shared/core/Strings.cpp
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
        src/client/ui/menus/GuessingMenu.cpp
        src/client/ui/menus/GuessingMenu.h
)

target_link_libraries(${PROJECT_NAME} SDL3 SDL2_image SDL2_mixer SDL2_ttf)
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
