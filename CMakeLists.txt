cmake_minimum_required(VERSION 3.29)
project(DrawSomething)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        C:/libs/SDL/include
        C:/libs/SDL_image/include
        C:/libs/SDL_mixer/include
        C:/libs/SDL_ttf/include
        C:/libs/asio/asio/include
        C:/libs/curl/include
        C:/libs/json/include
)

# Detect if we're building in Debug or Release mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SDL_LIB_DIR C:/libs/SDL/cmake-build-debug)
    set(SDL_IMAGE_LIB_DIR C:/libs/SDL_image/cmake-build-debug)
    set(SDL_MIXER_LIB_DIR C:/libs/SDL_mixer/cmake-build-debug)
    set(SDL_TTF_LIB_DIR C:/libs/SDL_ttf/cmake-build-debug)
else()
    set(SDL_LIB_DIR C:/libs/SDL/cmake-build-release)
    set(SDL_IMAGE_LIB_DIR C:/libs/SDL_image/cmake-build-release)
    set(SDL_MIXER_LIB_DIR C:/libs/SDL_mixer/cmake-build-release)
    set(SDL_TTF_LIB_DIR C:/libs/SDL_ttf/cmake-build-release)
endif()

# Add the chosen directories
link_directories(
        ${SDL_LIB_DIR}
        ${SDL_IMAGE_LIB_DIR}
        ${SDL_MIXER_LIB_DIR}
        ${SDL_TTF_LIB_DIR}
        C:/libs/curl/lib
)

#link_directories(
#        C:/libs/SDL/cmake-build-debug
#        C:/libs/SDL_image/cmake-build-debug
#        C:/libs/SDL_mixer/cmake-build-debug
#        C:/libs/SDL_ttf/cmake-build-debug
#        C:/libs/curl/lib
#)

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
        src/client/ui/menus/intermission/components/Scribbles.cpp
        src/client/ui/menus/intermission/components/Scribbles.h
        src/client/ui/components/element/Button.h
        src/client/ui/components/element/TextBox.cpp
        src/client/ui/components/element/base/Element.cpp
        src/client/ui/components/element/base/Element.h
        src/client/ui/components/element/Button.cpp
        src/client/ui/components/element/TextBox.h
        src/client/ui/components/event/EventContext.h
        src/client/ui/components/element/Frame.cpp
        src/client/ui/components/element/Frame.h
        src/client/ui/menus/main/MainMenu.cpp
        src/client/ui/menus/main/MainMenu.h
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
        src/client/ui/menus/ingame/components/panel/Letter.cpp
        src/client/ui/menus/ingame/components/panel/Letter.h
        src/client/ui/menus/ingame/components/panel/LetterSlot.cpp
        src/client/ui/menus/ingame/components/panel/LetterSlot.h
        src/client/ui/menus/intermission/IntermissionMenu.cpp
        src/client/ui/menus/intermission/IntermissionMenu.h
        src/client/ui/CommonUI.h
        src/client/ui/CommonUI.cpp
        src/client/ui/components/element/TextElement.cpp
        src/client/ui/components/element/TextElement.h
        src/client/ui/menus/intermission/components/MiniProfile.cpp
        src/client/ui/menus/intermission/components/MiniProfile.h
        src/client/ui/structures/visual_texture/VisualTexture.cpp
        src/client/ui/structures/visual_texture/VisualTexture.h
        src/client/ui/menus/main/components/Header.cpp
        src/client/ui/menus/main/components/Header.h
        src/client/ui/menus/main/components/Profile.cpp
        src/client/ui/menus/main/components/Profile.h
        src/client/ui/menus/main/components/games/CurrentGames.cpp
        src/client/ui/menus/main/components/games/CurrentGames.h
        src/client/ui/menus/main/components/Connections.cpp
        src/client/ui/menus/main/components/Connections.h
        src/client/ui/menus/main/components/games/game/Game.cpp
        src/client/ui/menus/main/components/games/game/Game.h
        src/client/ui/menus/main/components/games/game/Statistics.cpp
        src/client/ui/menus/main/components/games/game/Statistics.h
        src/client/ui/menus/picking/PickingMenu.cpp
        src/client/ui/menus/picking/PickingMenu.h
        src/client/ui/menus/picking/components/Header.cpp
        src/client/ui/menus/picking/components/Header.h
        src/client/ui/menus/main/components/SettingsScreen.cpp
        src/client/ui/menus/main/components/SettingsScreen.h
        src/client/ui/menus/loading/LoadingScreen.cpp
        src/client/ui/menus/loading/LoadingScreen.h
        src/client/game/game_info/GameInfo.cpp
        src/client/game/game_info/GameInfo.h
        src/client/game/game_info/GameStats.cpp
        src/client/game/game_info/GameStats.h
        src/client/ui/menus/ingame/components/tool_selector/DrawingToolButton.cpp
        src/client/ui/menus/ingame/components/tool_selector/DrawingToolButton.h
        src/client/ui/menus/confirmation/ConfirmationScreen.cpp
        src/client/ui/menus/confirmation/ConfirmationScreen.h
        src/client/ui/particles/Particles.cpp
        src/client/ui/particles/Particles.h
        src/client/ui/particles/particle/FlyingParticle.cpp
        src/client/ui/particles/particle/FlyingParticle.h
        src/client/core/Rectangles.inl
        src/client/ui/components/element/ScrollFrame.cpp
        src/client/ui/components/element/ScrollFrame.h
        src/client/ui/components/element/CheckBox.cpp
        src/client/ui/components/element/CheckBox.h
        src/client/ui/components/element/Scrollable.cpp
        src/client/ui/components/element/Scrollable.h
        src/client/ui/structures/visual_texture/files/HitboxFile.cpp
        src/client/ui/structures/visual_texture/files/HitboxFile.h
        src/client/ui/menus/auth/components/ScrollingBackground.cpp
        src/client/ui/menus/auth/components/ScrollingBackground.h
        src/client/network/NetworkClient.cpp
        src/client/network/NetworkClient.h
        src/client/ui/menus/picking/components/WordOption.cpp
        src/client/ui/menus/picking/components/WordOption.h
        src/client/game/account/Account.cpp
        src/client/game/account/Account.h
        src/client/game/GameData.cpp
        src/client/game/GameData.h
        src/client/ui/menus/main/admin/AdminScreen.cpp
        src/client/ui/menus/main/admin/AdminScreen.h
        src/client/ui/menus/main/admin/UserRecord.cpp
        src/client/ui/menus/main/admin/UserRecord.h
        src/client/ui/menus/main/shop/ShopScreen.cpp
        src/client/ui/menus/main/shop/ShopScreen.h
        src/client/ui/menus/main/shop/bundle/ColorBundle.cpp
        src/client/ui/menus/main/shop/bundle/ColorBundle.h
        src/client/ui/menus/main/shop/bundle/ColorView.h
        src/client/ui/menus/main/shop/bundle/ColorView.cpp
        src/client/ui/RenderPresets.h
        src/client/ui/RenderPresets.cpp
        src/client/ui/menus/main/profile/ProfileScreen.h
        src/client/ui/menus/main/profile/ProfileScreen.cpp
        src/client/ui/menus/ScreenMenu.h
        src/client/ui/menus/ScreenMenu.cpp
)

add_executable(DrawSomethingEditor src/client/editor.cpp
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
        src/client/ui/components/element/Button.cpp
        src/client/ui/components/element/Button.h
        src/client/ui/components/element/TextBox.cpp
        src/client/ui/components/element/TextBox.h
        src/client/ui/components/element/base/Element.cpp
        src/client/ui/components/element/base/Element.h
        src/client/ui/components/event/EventContext.h
        src/client/ui/components/element/Frame.cpp
        src/client/ui/components/element/Frame.h
        src/client/core/Rectangles.h
        src/client/ui/menus/FullscreenMenu.cpp
        src/client/ui/menus/FullscreenMenu.h
        src/client/ui/components/element/OverlayButton.cpp
        src/client/ui/components/element/OverlayButton.h
        src/client/ui/cursors/Circular.cpp
        src/client/ui/cursors/Circular.h
        src/client/ui/cursors/Cursors.h
        src/client/ui/cursors/Cursors.cpp
        src/client/ui/structures/WrappedText.cpp
        src/client/ui/structures/WrappedText.h
        src/client/ui/structures/window_texture/WindowTexture.cpp
        src/client/ui/structures/window_texture/WindowTexture.h
        src/client/ui/structures/window_texture/Sheet.cpp
        src/client/ui/structures/window_texture/Sheet.h
        src/client/ui/CommonUI.h
        src/client/ui/CommonUI.cpp
        src/client/ui/components/element/TextElement.cpp
        src/client/ui/components/element/TextElement.h
        src/client/ui/structures/visual_texture/VisualTexture.cpp
        src/client/ui/structures/visual_texture/VisualTexture.h
        src/client/editor/ui/menus/editor/Interface.cpp
        src/client/editor/ui/menus/editor/Interface.h
        src/shared/core/Numbers.h
        src/shared/core/Colors.cpp
        src/shared/core/Colors.h
        src/client/editor/ui/menus/editor/components/texture_button/TextureButton.cpp
        src/client/editor/ui/menus/editor/components/texture_button/TextureButton.h
        src/client/ui/components/element/ScrollFrame.cpp
        src/client/ui/components/element/ScrollFrame.h
        src/client/editor/ui/menus/editor/components/viewport/Viewport.cpp
        src/client/editor/ui/menus/editor/components/viewport/Viewport.h
        src/client/editor/ui/menus/editor/components/viewport/ViewportItem.cpp
        src/client/editor/ui/menus/editor/components/viewport/ViewportItem.h
        src/client/ui/components/element/CheckBox.cpp
        src/client/ui/components/element/CheckBox.h
        src/client/ui/components/element/Scrollable.cpp
        src/client/ui/components/element/Scrollable.h
        src/client/ui/structures/visual_texture/files/HitboxFile.cpp
        src/client/ui/structures/visual_texture/files/HitboxFile.h
        src/client/editor/ui/menus/editor/components/texture_button/TextureStateIndicator.cpp
        src/client/editor/ui/menus/editor/components/texture_button/TextureStateIndicator.h
)

target_include_directories(DrawSomething PRIVATE src)
target_link_libraries(${PROJECT_NAME} SDL3 SDL3_image SDL3_mixer SDL3_ttf ws2_32 libcurl.a curl)
target_link_options(${PROJECT_NAME} PRIVATE "-static")

target_include_directories(DrawSomethingEditor PRIVATE src)
target_link_libraries(DrawSomethingEditor SDL3 SDL3_image SDL3_mixer SDL3_ttf)
target_link_options(DrawSomethingEditor PRIVATE "-static")

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
